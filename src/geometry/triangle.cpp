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
	Vector3f Triangle::le(Interaction* p, float& pdf)
	{
		Vector3f p2light = p->p - p->ray->origin;
		cout << "light> p=" << vector3fToString(p->p) << endl;
		cout << "light> light_point=" << vector3fToString(p->ray->origin) << endl;
		cout << "light> p2light=" << vector3fToString(p2light) << endl;
		cout << "light> p2light(normalized)=" << vector3fToString(p2light.normalized()) << endl;
		if (singleSide && p2light.dot(outsideNormal) > 0) {
			pdf = 0;
			return { 0, 0, 0 };
		}
		float dir = p2light.norm();
		cout << "light> dir=" << dir << endl;
		float abscostheta = abs(p->normal.dot(p2light.normalized()));
		cout << "light> pNormal=" << vector3fToString(p->normal) << endl;
		pdf = dir * dir / (abscostheta * area());
		cout << "light> abscostheta=" << abscostheta << endl;
		cout << "light> area=" << area() << endl;
		return emitLight;
	}
	float Triangle::area()
	{
		float e1 = (p1 - p2).norm(),
			e2 = (p2 - p3).norm(),
			e3 = (p1 - p3).norm();
		float pt = (e1 + e2 + e3) / 2.f;
		return sqrt(pt * (pt - e1) * (pt - e2) * (pt - e3));
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