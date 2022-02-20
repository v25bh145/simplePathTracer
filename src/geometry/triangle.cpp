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
		}
		this->texture = texture;
		p1UV = { uvArray[0].x(), uvArray[0].y() };
		p2UV = { uvArray[1].x(), uvArray[1].y() };
		p3UV = { uvArray[2].x(), uvArray[2].y() };
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