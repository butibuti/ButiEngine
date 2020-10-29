#include"stdafx.h"


ButiEngine::Calculator::Calculator()
{
}


ButiEngine::Calculator::~Calculator()
{
}

bool ButiEngine::Calculator::IsClossLine(Vector2 a, Vector2 b, Vector2 c, Vector2 d)
{
	return ((((a.x - b.x)*(c.y - a.y) + (a.y - b.y)*(a.x - c.x))
		*    ((a.x - b.x)*(d.y - a.y) + (a.y - b.y)*(a.x - d.x)) < 0) &&
		(((c.x - d.x)*(a.y - c.y) + (c.y - d.y)*(c.x - a.x))
			*((c.x - d.x)*(b.y - c.y) + (c.y - d.y)*(c.x - b.x)) < 0)
		);
}


float ButiEngine::Calculator::Abs(float value)
{
	if(value>0)
	return value;
	else
	{
		return value* -1;
	}
}

int ButiEngine::Calculator::Abs(int value)
{
	if (value > 0)
		return value;
	else
	{
		return value * -1;
	}
}

double ButiEngine::Calculator::Abs(double value)
{
	if (value > 0)
		return value;
	else
	{
		return value * -1;
	}
}

float ButiEngine::Calculator::Distance(ButiEngine::Vector3 pointA, Vector3 pointB)
{
	float length = sqrt((pointA.x-pointB.x)*(pointA.x - pointB.x) + (pointA.y - pointB.y)*(pointA.y - pointB.y) + (pointA.z - pointB.z)*(pointA.z - pointB.z));
	return length;
}

ButiEngine::Vector3 ButiEngine::Calculator::Normalize(Vector3 value)
{
	value.Normalize();
	return value;
}

float ButiEngine::Calculator::ToRadian(float deg)
{
	return deg*PI/180.0f;
}

float ButiEngine::Calculator::ToDegree(float rad)
{
	return rad*180.0f/PI;
}

ButiEngine::Vector2 ButiEngine::Calculator::Float2Verticalize(Vector2 vector2)
{
	return Vector2(vector2.y,-vector2.x);
}

ButiEngine::Vector2 ButiEngine::Calculator::Normalize2D(Vector2 vector2)
{
	float length = sqrt(vector2.x*vector2.x +vector2.y*vector2.y );
	return Vector2(vector2.x / length, vector2.y / length);
}

ButiEngine::Vector2 ButiEngine::Calculator::Float2Addition(Vector2 a, Vector2 b)
{
	auto c = Vector2();
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	return c;
}

ButiEngine::Vector2 ButiEngine::Calculator::Float2Multiplication(Vector2 a, float scalar)
{
	return Vector2(a.x*scalar,a.y*scalar);
}

float ButiEngine::Calculator::GetCross2D(Vector2 a, Vector2 b, Vector2 c)
{
	auto vectorab = Vector2(b.x - a.x, b.y - a.y);
	auto vectorac = Vector2(c.x - a.x, c.y - a.y);
	//+ is left; - is right;
	return vectorab.x*vectorac.y - vectorac.x*vectorab.y;
}

ButiEngine::Vector3 ButiEngine::Calculator::ScreenPointToWorldPoint(Vector2 screenPos, float z, Vector2 screenRect, DirectX::XMMATRIX * p_view, DirectX::XMMATRIX * p_proj)
{
	/*screenPos.x /= screenRect.x;
	screenPos.y /= screenRect.y;*/
	DirectX::XMMATRIX InvView, InvPrj, VP, InvViewport;
	auto v_view = *p_view;
	auto v_proj = *p_proj;
	v_view = DirectX::XMMatrixTranspose(*p_view);
	v_proj = DirectX::XMMatrixTranspose(*p_proj);
	InvView= DirectX::XMMatrixInverse(nullptr,v_view);
	InvPrj= DirectX::XMMatrixInverse(nullptr, v_proj);

	VP= DirectX::XMMatrixIdentity();
	DirectX::XMFLOAT4X4 vp;
	DirectX::XMStoreFloat4x4(&vp, VP);
	vp.m[0][0] = screenRect.x / 2.0f; vp.m[1][1] = -screenRect.y / 2.0f;
	vp.m[3][0] = screenRect.x / 2.0f; vp.m[3][1] = screenRect.y / 2.0f;

	VP= DirectX::XMLoadFloat4x4(&vp);

	InvViewport=DirectX::XMMatrixInverse(nullptr, VP);

	DirectX::XMVECTOR changeVector=DirectX::XMLoadFloat3(&Vector3( screenPos.x,screenPos.y,z));

	// inverseConvert
	DirectX::XMMATRIX tmp = InvViewport * InvPrj * InvView;
	DirectX::XMVECTOR pout= DirectX::XMVector3TransformCoord(changeVector,tmp);
	
	Vector3 output;
	DirectX::XMStoreFloat3(&output, pout);
	return output;
}

ButiEngine::Vector3 ButiEngine::Calculator::ScreenPointToXYPlane(Vector2 screenPos, Vector2 screenRect, DirectX::XMMATRIX * p_view, DirectX::XMMATRIX * p_proj)
{

	Vector3 output,wall;
	Vector3 nearPos, farPos, ray;
	wall = Vector3(0, 0, 1);
	nearPos = ScreenPointToWorldPoint(screenPos, 0.0f, screenRect, p_view, p_proj);
	farPos = ScreenPointToWorldPoint(screenPos, 1.0f, screenRect, p_view, p_proj);
	ray = Vector3(farPos.x - nearPos.x, farPos.y - nearPos.y, farPos.z - nearPos.z);
	ray.Normalize();
	if (ray.z >= 0) {
		DirectX::XMVECTOR rayVector = DirectX::XMLoadFloat3(&ray);
		DirectX::XMVECTOR wallVector = DirectX::XMLoadFloat3(&wall);
		nearPos= (nearPos* -1.0f);
		DirectX::XMVECTOR nearVector = DirectX::XMLoadFloat3(&nearPos);
		float lRay; DirectX::XMStoreFloat(&lRay, DirectX::XMVector3Dot(rayVector, wallVector));
		float l0; DirectX::XMStoreFloat(&l0, DirectX::XMVector3Dot(nearVector, wallVector));
		Vector3 r;
		DirectX::XMStoreFloat3(&r, rayVector);
		r= r* (l0 / lRay);
		DirectX::XMStoreFloat3(&nearPos, nearVector);
		output = nearPos+ r;
	}
	else
	{
		output = farPos;
	}
	output.z = 0;
	return output;
}

std::vector<ButiEngine::Vector2> ButiEngine::Calculator::GetPerimeter(std::vector<Vector2> poses)
{
	if (poses.size() <= 0)
		return std::vector<Vector2>();
	std::vector<Vector2> vertexes;

	auto start = poses.begin();//StartPoint
	for (auto itr = poses.begin(); itr != poses.end(); itr++) {
		if (itr->x < start->x)start = itr;
	}
	vertexes.push_back(*start);
	auto v_itr = 0;//currentPoint

	while (true)
	{
		auto next = poses.begin();
		if ((*next).x == (vertexes.at(v_itr)).x&& (*next).y == vertexes.at(v_itr).y)
			next++;
		for (auto check = poses.begin(); check != poses.end(); check++) {
			if (GetCross2D(vertexes.at(v_itr), *next, *check) > 0) {
				next = check;
			}
		}

		if (start == next) {
			break;
		}
		vertexes.push_back(*next);
		v_itr++;
	}

	return vertexes;
}

char ButiEngine::Calculator::GetByteSize(const int arg_check)
{
	if (arg_check<= CharMax) {
		return 1;
	}
	else if (arg_check<=ShortMax) {
		return 2;
	}
	else if (arg_check<= IntMax) {
		return 4;
	}
}

char ButiEngine::Calculator::GetUnsignedByteSize(const UINT arg_check)
{
	if (arg_check <= CharMax*2) {
		return 1;
	}
	else if (arg_check <= ShortMax*2) {
		return 2;
	}
	else if (arg_check <= IntMax*2) {
		return 4;
	}
}
