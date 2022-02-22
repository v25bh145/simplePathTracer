#include "triangle.h"
namespace pathTracer {
	Geometry* Triangle::hasSubGeometriesId(unsigned int id)
	{
		if (RTCInnerGeometryId == id) return this;
		else return nullptr;
	}
	Vector3f Triangle::sample_point(float& pdf)
	{
		pdf = 1 / area();
		RandomGenerator randomGenerator;
		float u = randomGenerator.uniform0To1(),
			  v = randomGenerator.uniform0To1();
		Vector3f point = u * p1 + v * p2 + (1.f - u - v) * p3;
		// bilerp interpolation
		return point;
	}
	Vector3f Triangle::le(Interaction* p1, Interaction* pLight, float& wi_pdf)
	{
		// wi_pdf = A_pdf * d * d / cos
		Vector3f p2light = pLight->p - p1->p;
		//cout << "light> light_point=" << vector3fToString(pLight->p) << endl;
		//cout << "light> p=" << vector3fToString(p1->p) << endl;
		//cout << "light> p2light=" << vector3fToString(p2light) << endl;
		//cout << "light> p2light(normalized)=" << vector3fToString(p2light.normalized()) << endl;
		if (singleSide && p2light.dot(outsideNormal) > 0) {
			wi_pdf = 0;
			return { 0, 0, 0 };
		}
		float dir = p2light.norm();
		//cout << "light> p2light(dir)=" << dir << endl;
		float abscostheta = abs(pLight->normal.dot(p2light.normalized()));
		//cout << "light> lightNormal=" << vector3fToString(pLight->normal) << endl;
		//cout << "light> abscostheta=" << abscostheta << endl;
		//cout << "light> area=" << area() << endl;
		//wi_pdf = dir * dir / (area() * abscostheta);
		wi_pdf = dir * dir / (area() * abscostheta);
		//cout << "light> wi_pdf=" << wi_pdf << endl;
		return emitLight;
	}
	float Triangle::area()
	{
		return triangleArea(p1, p2, p3);
	}
	void Triangle::loadRealGeometryFlush()
	{
		rtcCommitGeometry(*RTCInnerGeometry);
	}
	void Triangle::attachAllGeometriesToScene(Scene scene)
	{
		rtcAttachGeometryByID(*scene.RTCInnerScene, *RTCInnerGeometry, RTCInnerGeometryId);
	}
	unsigned Triangle::getRTCInnerGeometryId()
	{
		return this->RTCInnerGeometryId;
	}
	RTCGeometry* Triangle::getRTCInnerGeometry()
	{
		return this->RTCInnerGeometry;
	}
	Vector3f Triangle::getOutsideNormal()
	{
		return outsideNormal;
	}
	void Triangle::deepCopy(Geometry*& geometry)
	{
		geometry = new Triangle();
		Triangle* triangle = (Triangle*)geometry;
		triangle->p1 = this->p1;
		triangle->p2 = this->p2;
		triangle->p3 = this->p3;
		triangle->emitLight = this->emitLight;
		triangle->outsideNormal = this->outsideNormal;
		triangle->singleSide = this->singleSide;
		triangle->bxdf = nullptr;
		this->bxdf->deepCopy(triangle->bxdf);
		triangle->RTCInnerGeometry = this->RTCInnerGeometry;
		triangle->RTCInnerGeometryId = this->RTCInnerGeometryId;
		triangle->insideMedium = triangle->outsideMedium = nullptr;
		if (insideMedium != nullptr)
			this->insideMedium->deepCopy(triangle->insideMedium);
		if (outsideMedium != nullptr)
			this->outsideMedium->deepCopy(triangle->outsideMedium);
		if (texture != nullptr)
			this->texture->deepCopy(triangle->texture);
		triangle->p1UV = this->p1UV;
		triangle->p2UV = this->p2UV;
		triangle->p3UV = this->p3UV;
		triangle->dpdu = this->dpdu;
		triangle->dpdv = this->dpdv;
		triangle->texelSize = this->texelSize;
	}
	Medium* Triangle::getOutsideMedium()
	{
		return this->outsideMedium;
	}

	Medium* Triangle::getInsideMedium()
	{
		return this->insideMedium;
	}
	void Triangle::attachTexture(Texture2D* texture, vector<Vector2f> uvArray)
	{
		if (uvArray.size() < 3) {
			cout << "error: texture UV input format is wrong!" << endl;
			assert(false);
		}
		this->texture = texture;
		p1UV = uvArray[0];
		p2UV = uvArray[1];
		p3UV = uvArray[2];
		// calculate dpdu & dpdv
		// partialp/partialu * du + partialp/partialv * dv = dp [derivative]
		Matrix2f duv;
		duv << p1UV.x() - p3UV.x(), p2UV.x() - p3UV.x(), 
			   p1UV.y() - p3UV.y(), p2UV.y() - p3UV.y();
		pair<Vector3f, Vector3f> dp = { p1 - p3, p2 - p3 };
		float det = duv.determinant();
		if (abs(det) < EPSILON) {
			// select two orthogonal directions
			orthogonal(outsideNormal, this->dpdu, this->dpdv);
		}
		else {
			float invDet = 1.f / det;
			this->dpdu = (duv(1, 1) * dp.first - duv(1, 0) * dp.second) * invDet;
			this->dpdv = (-duv(0, 1) * dp.first + duv(0, 0) * dp.second) * invDet;
			cout << "dpdu=" << vector3fToString(dpdu) << ", dpdv=" << vector3fToString(dpdv) << endl;
		}
		// texelSize
		float objectLengthP1P2 = (p1 - p2).norm();
		float dpUVX = p1UV.x() - p2UV.x();
		float dpUVY = p1UV.y() - p2UV.y();
		float textureLengthP1P2 = sqrt(dpUVX * dpUVX * texture->width * texture->width + dpUVY * dpUVY * texture->height * texture->height);
		this->texelSize = objectLengthP1P2 / textureLengthP1P2;
		cout << "texelSize=" << texelSize << endl;
	}
	Vector2f Triangle::getUV(Vector3f p)
	{
		if (texture == nullptr) return { 0.f, 0.f };
		Vector3f barycentric;
		float areaP1P2P3 = this->area();
		float areaPP1P2 = triangleArea(p, p1, p2);
		float areaPP2P3 = triangleArea(p, p2, p3);
		barycentric.x() = areaPP2P3 / areaP1P2P3;
		barycentric.z() = areaPP1P2 / areaP1P2P3;
		barycentric.y() = 1.f - barycentric.x() - barycentric.z();
		return barycentric.x() * p1UV + barycentric.y() * p2UV + barycentric.z() * p3UV;
	}
	pair<Vector3f, Vector3f> Triangle::getdpduv(Vector3f p)
	{
		if (texture == nullptr) {
			cout << "ERROR: can't call getdpduv() from a geometry which hasn't texture" << endl;
			assert(false);
		}
		return {dpdu, dpdv};
	}
	Texture2D* Triangle::getTexture()
	{
		return texture;
	}
	float Triangle::getTexelSize(Vector3f p)
	{
		if (texture == nullptr) {
			cout << "ERROR: can't call getTexelSize() from a geometry which hasn't texture" << endl;
			assert(false);
		}
		return texelSize;
	}
	string Triangle::toString()
	{
		string info = "";
		ostringstream buffer(info);
		buffer << "id=" << this->RTCInnerGeometryId << ", type=triangle" << endl;
		if (this->insideMedium)
			buffer << "insideMedium = " << this->insideMedium->toString() << endl;
		else
			buffer << "insideMedium = nullptr" << endl;

		if (this->outsideMedium)
			buffer << "outsideMedium = " << this->outsideMedium->toString() << endl;
		else
			buffer << "outsideMedium = nullptr" << endl;
		return buffer.str();
	}
}