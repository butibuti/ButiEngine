#include"stdafx.h"
#include"Header/Device/Helper/MeshHelper.h"
#include "..\..\..\Header\Device\Helper\MeshHelper.h"
ButiEngine::MeshHelper::~MeshHelper()
{
}


void ButiEngine::MeshHelper::ReverseWinding(std::vector<Vertex::Vertex_UV_Normal_Color>& ref_vertices, std::vector<UINT>& ref_indices)
{
	if ((ref_indices.size() % 3) != 0)
		return;
	for (auto it = ref_indices.begin(); it != ref_indices.end(); it += 3)
	{
		std::swap(*it, *(it + 2));
	}
	for (auto it = ref_vertices.begin(); it != ref_vertices.end(); ++it)
	{
		it->uv.x = (1.f - it->uv.x);
	}
}


DirectX::XMVECTOR ButiEngine::MeshHelper::GetCircleTangent(int i, int tessellation)
{
	float angle = (i * DirectX::XM_2PI / tessellation) + DirectX::XM_PIDIV2;
	float dx, dz;

	DirectX::XMScalarSinCos(&dx, &dz, angle);

	DirectX::XMVECTORF32 v = { dx, 0, dz, 0 };
	return v;
}

DirectX::XMVECTOR ButiEngine::MeshHelper::GetCircleVector(int size, int tessellation)
{
	float angle = size * DirectX::XM_2PI / tessellation;
	float dx, dz;

	DirectX::XMScalarSinCos(&dx, &dz, angle);

	DirectX::XMVECTORF32 v = { dx, 0, dz, 0 };
	return v;

}



void ButiEngine::MeshHelper::CreateTriangle(Vector3 point1, Vector3 point2, Vector3 point3, const std::vector<Color>& arg_colors, bool flat, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData)
{
	std::vector<Vertex::Vertex_UV_Normal_Color> vertices{

	   Vertex::Vertex_UV_Normal_Color(Vector3(0.0f, 0.5f, 0.0f), Vector2(0.5f, 0.0f)),
	   Vertex::Vertex_UV_Normal_Color(Vector3(0.5f, -0.5f, 0.0f), Vector2(1.0f, 1.0f)),
		   Vertex::Vertex_UV_Normal_Color(Vector3(-0.5f, -0.5f, 0.0f), Vector2(0.0f, 1.0f)) };
	std::vector<UINT>indices{
		0,1,2
	};
	VertexFlatConverter(vertices);
	VertexAttachColor(arg_colors, vertices);
	outputMeshData.vertices = vertices;
	outputMeshData.indices = indices;
}

void ButiEngine::MeshHelper::CreateSameTextureCube(Vector3 size, const std::vector<Color>& arg_colors,bool flat, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData)
{
	enum class ColorAttachType {
		SingleColor, AllColor, FourColor, SixColor
	};
	ColorAttachType colorType = ColorAttachType::SingleColor;
	if (arg_colors.size() == 4)colorType = ColorAttachType::FourColor;
	else if (arg_colors.size() == 6)colorType = ColorAttachType::SixColor;
	size.x *= 0.5f;
	size.y *= 0.5f;
	size.z *= 0.5f;

	std::vector<Vector3> PosVec = {
			{ Vector3(-size.x, size.y, -size.z) },
			{ Vector3(size.x,size.y, -size.z) },
			{ Vector3(-size.x, -size.y, -size.z) },
			{ Vector3(size.x, -size.y, -size.z) },
			{ Vector3(size.x,size.y, size.z) },
			{ Vector3(-size.x,size.y, size.z) },
			{ Vector3(size.x, -size.y, size.z) },
			{ Vector3(-size.x, -size.y, size.z) },
	};
	std::vector<UINT> PosIndeces = {
		0, 1, 2, 3,
		1, 4, 3, 6,
		4, 5, 6, 7,
		5, 0, 7, 2,
		5, 4, 0, 1,
		2, 3, 7, 6,
	};
	std::vector<Vector2> vec_uv = {
		Vector2(0,0),
		Vector2(1,0),
		Vector2(0,1),
		Vector2(1,1),
	};

	std::vector<Vector3> FaceNormalVec = {
		{ Vector3(0, 0, -1.0f) },
		{ Vector3(1.0f, 0, 0) },
		{ Vector3(0, 0, 1.0f) },
		{ Vector3(-1.0f, 0, 0) },
		{ Vector3(0, 1.0f, 0) },
		{ Vector3(0, -1.0f, 0) }
	};

	std::vector<Vertex::Vertex_UV_Normal_Color> vertices;
	std::vector<UINT> indices;
	UINT BasePosCount = 0;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			Vertex::Vertex_UV_Normal_Color Data;
			Data.position = PosVec[PosIndeces[BasePosCount + j]];
			Data.uv = vec_uv.at(j);
			if (flat) {
				//フラット表示の場合は法線は頂点方向にする
				Data.normal = Data.position;
				Data.normal.Normalize();
			}
			else {
				//フラット表示しない場合は、法線は面の向き
				Data.normal = FaceNormalVec[i];
			}
			if (colorType == ColorAttachType::SixColor) {
				Data.color = arg_colors.at(i);
			}
			else if (colorType == ColorAttachType::FourColor) {
				Data.color = arg_colors.at(j);

			}
			vertices.push_back(Data);
		}

		indices.push_back((UINT)BasePosCount + 0);
		indices.push_back((UINT)BasePosCount + 1);
		indices.push_back((UINT)BasePosCount + 2);
		indices.push_back((UINT)BasePosCount + 1);
		indices.push_back((UINT)BasePosCount + 3);
		indices.push_back((UINT)BasePosCount + 2);

		BasePosCount += 4;

	}
	if(colorType==ColorAttachType::SingleColor)
	VertexAttachColor(arg_colors, vertices);

	outputMeshData.vertices = vertices;
	outputMeshData.indices = indices;
}

void ButiEngine::MeshHelper::CreateCube(Vector3 size, const std::vector<Color>& arg_colors, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData, bool flat)
{
	enum class ColorAttachType {
	SingleColor,AllColor,FourColor,SixColor
	};
	ColorAttachType colorType=ColorAttachType::SingleColor;
	if (arg_colors.size() == 4)colorType = ColorAttachType::FourColor;
	else if (arg_colors.size() == 6)colorType = ColorAttachType::SixColor;
	size*= 0.5f;

	std::vector<Vector3> PosVec = {
			{ Vector3(-size.x, size.y, -size.z) },
			{ Vector3(size.x,size.y, -size.z) },
			{ Vector3(-size.x, -size.y, -size.z) },
			{ Vector3(size.x, -size.y, -size.z) },
			{ Vector3(size.x,size.y, size.z) },
			{ Vector3(-size.x,size.y, size.z) },
			{ Vector3(size.x, -size.y, size.z) },
			{ Vector3(-size.x, -size.y, size.z) },
	};
	std::vector<UINT> PosIndeces = {
		0, 1, 2, 3,
		1, 4, 3, 6,
		4, 5, 6, 7,
		5, 0, 7, 2,
		5, 4, 0, 1,
		2, 3, 7, 6,
	};
	std::vector<Vector2> vec_uv = {
		Vector2(0,0),
		Vector2(1,0),
		Vector2(0,1),
		Vector2(1,1),
	};

	std::vector<Vector3> FaceNormalVec = {
		{ Vector3(0, 0, -1.0f) },
		{ Vector3(1.0f, 0, 0) },
		{ Vector3(0, 0, 1.0f) },
		{ Vector3(-1.0f, 0, 0) },
		{ Vector3(0, 1.0f, 0) },
		{ Vector3(0, -1.0f, 0) }
	};

	std::vector<Vertex::Vertex_UV_Normal_Color> vertices;
	std::vector<UINT> indices;
	UINT BasePosCount = 0;
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			Vertex::Vertex_UV_Normal_Color Data;
			Data.position = PosVec[PosIndeces[BasePosCount + j]];
			Data.uv = vec_uv.at(j);
			if (flat) {
				//フラット表示の場合は法線は頂点方向にする
				Data.normal = Data.position;
				Data.normal.Normalize();
			}
			else {
				//フラット表示しない場合は、法線は面の向き
				Data.normal = FaceNormalVec[i];
			}
			if (colorType == ColorAttachType::SixColor) {
				Data.color = arg_colors.at(i);
			}
			else if(colorType==ColorAttachType::FourColor){
				Data.color = arg_colors.at(j);

			}
			vertices.push_back(Data);
		}

		indices.push_back((UINT)BasePosCount + 0);
		indices.push_back((UINT)BasePosCount + 1);
		indices.push_back((UINT)BasePosCount + 2);
		indices.push_back((UINT)BasePosCount + 1);
		indices.push_back((UINT)BasePosCount + 3);
		indices.push_back((UINT)BasePosCount + 2);

		BasePosCount += 4;

	}

	outputMeshData.eightCorner.up_left_back = Vector3(-size.x, size.y, -size.z);
	outputMeshData.eightCorner.up_left_front = Vector3(-size.x, size.y, size.z);
	outputMeshData.eightCorner.up_right_back = Vector3(size.x, size.y, -size.z);
	outputMeshData.eightCorner.up_right_front = Vector3(size.x, size.y, size.z);

	outputMeshData.eightCorner.down_left_back = Vector3(-size.x, -size.y, -size.z);
	outputMeshData.eightCorner.down_left_front = Vector3(-size.x, -size.y, size.z);
	outputMeshData.eightCorner.down_right_back = Vector3(size.x, -size.y, -size.z);
	outputMeshData.eightCorner.down_right_front = Vector3(size.x, -size.y, size.z);

	if (colorType == ColorAttachType::SingleColor)
		VertexAttachColor(arg_colors, vertices);
	outputMeshData.vertices = vertices;
	outputMeshData.indices = indices;
}


void ButiEngine::MeshHelper::CreateSphere(Vector3 size, int tessellation, const std::vector<Color>& arg_colors, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData)
{
	if (tessellation < 3) {
		CreateCube(size, arg_colors, outputMeshData, true);
		return;
	}
	size = size * 0.5f;
	std::vector< Vertex::Vertex_UV_Normal_Color> vertices;
	std::vector<UINT> indices;

	outputMeshData.eightCorner.up_left_back = Vector3(-size.x,  size.y, -size.z);
	outputMeshData.eightCorner.up_left_front = Vector3(-size.x, size.y, size.z);
	outputMeshData.eightCorner.up_right_back = Vector3( size.x, size.y, -size.z);
	outputMeshData.eightCorner.up_right_front = Vector3(size.x, size.y, size.z);

	outputMeshData.eightCorner.down_left_back =  Vector3(-size.x,-size.y, -size.z);
	outputMeshData.eightCorner.down_left_front = Vector3(-size.x,-size.y, size.z);
	outputMeshData.eightCorner.down_right_back = Vector3(size.x, -size.y, -size.z);
	outputMeshData.eightCorner.down_right_front =Vector3(size.x, -size.y, size.z);

	size_t verticalSegments = tessellation;
	size_t horizontalSegments = tessellation * 2;
	for (size_t i = 0; i <= verticalSegments; i++)
	{


		float v = 1 - (float)i / verticalSegments;

		float latitude = (i * DirectX::XM_PI / verticalSegments) - DirectX::XM_PIDIV2;
		float dy, dxz;

		DirectX::XMScalarSinCos(&dy, &dxz, latitude);

		Vector3 CenterPos;



		for (size_t j = 0; j <= horizontalSegments; j++)
		{
			float u = (float)j / horizontalSegments;

			float longitude = j * DirectX::XM_2PI / horizontalSegments;
			float dx, dz;

			DirectX::XMScalarSinCos(&dx, &dz, longitude);

			dx *= dxz;
			dz *= dxz;

			DirectX::XMVECTOR normal = DirectX::XMVectorSet(dx, dy, dz, 0);
			auto pos = Vector3(dx * size.x, dy * size.y, dz * size.z);
			vertices.push_back(Vertex::Vertex_UV_Normal_Color(pos, Vector2(u, v), pos.GetNormalize()));
		}
	}
	size_t stride = horizontalSegments + 1;
	for (size_t i = 0; i < verticalSegments; i++)
	{
		for (size_t j = 0; j <= horizontalSegments; j++)
		{
			size_t nextI = i + 1;
			size_t nextJ = (j + 1) % stride;

			indices.push_back((UINT)(i * stride + j));
			indices.push_back((UINT)(nextI * stride + j));
			indices.push_back((UINT)(i * stride + nextJ));

			indices.push_back((UINT)(i * stride + nextJ));
			indices.push_back((UINT)(nextI * stride + j));
			indices.push_back((UINT)(nextI * stride + nextJ));
		}
	}
	ReverseWinding(vertices, indices);



	outputMeshData.vertices = vertices;
	outputMeshData.indices = indices;
}

void ButiEngine::MeshHelper::CreateSphereForParticle(Vector3 size, int tessellation, const std::vector<Color>& arg_colors, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData)
{
	if (tessellation < 3) {
		CreateCube(size, arg_colors, outputMeshData, true);
		return;
	}
	size = size * 0.5f;
	std::vector< Vertex::Vertex_UV_Normal_Color> vertices;
	std::vector<UINT> indices;

	outputMeshData.eightCorner.up_left_back = Vector3(-size.x, size.y, -size.z);
	outputMeshData.eightCorner.up_left_front = Vector3(-size.x, size.y, size.z);
	outputMeshData.eightCorner.up_right_back = Vector3(size.x, size.y, -size.z);
	outputMeshData.eightCorner.up_right_front = Vector3(size.x, size.y, size.z);

	outputMeshData.eightCorner.down_left_back = Vector3(-size.x, -size.y, -size.z);
	outputMeshData.eightCorner.down_left_front = Vector3(-size.x, -size.y, size.z);
	outputMeshData.eightCorner.down_right_back = Vector3(size.x, -size.y, -size.z);
	outputMeshData.eightCorner.down_right_front = Vector3(size.x, -size.y, size.z);

	size_t verticalSegments = tessellation;
	size_t horizontalSegments = tessellation * 2;
	for (size_t i = 0; i <= verticalSegments; i++)
	{


		float v = 1 - (float)i / verticalSegments;

		float latitude = (i * DirectX::XM_PI / verticalSegments) - DirectX::XM_PIDIV2;
		float dy, dxz;

		DirectX::XMScalarSinCos(&dy, &dxz, latitude);

		Vector3 CenterPos;

		if (i == 0||i== verticalSegments) {
			auto pos = Vector3(0, dy * size.y, 0);
			vertices.push_back(Vertex::Vertex_UV_Normal_Color(pos, Vector2(0, v), pos.GetNormalize()));
			continue;
		}

		for (size_t j = 0; j <= horizontalSegments; j++)
		{
			float u = (float)j / horizontalSegments;

			float longitude = j * DirectX::XM_2PI / horizontalSegments;
			float dx, dz;

			DirectX::XMScalarSinCos(&dx, &dz, longitude);

			dx *= dxz;
			dz *= dxz;

			auto pos = Vector3(dx * size.x, dy * size.y, dz * size.z);
			vertices.push_back(Vertex::Vertex_UV_Normal_Color(pos, Vector2(u, v), pos.GetNormalize()));
		}
	}
	
	for (UINT i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}


	outputMeshData.vertices = vertices;
	outputMeshData.indices = indices;
}

void ButiEngine::MeshHelper::CreateCylinderCap(const std::vector<Color>& arg_colors, std::vector<Vertex::Vertex_UV_Normal_Color>& ref_vertices, std::vector<UINT>& ref_indices, Vector3 size, int tessellation, bool isTop)
{
	// indices.
	for (size_t i = 0; i < tessellation - 2; i++)
	{
		size_t i1 = (i + 1) % tessellation;
		size_t i2 = (i + 2) % tessellation;

		if (isTop)
		{
			std::swap(i1, i2);
		}

		size_t vbase = ref_vertices.size();
		ref_indices.push_back((UINT)vbase);
		ref_indices.push_back((UINT)(vbase + i1));
		ref_indices.push_back((UINT)(vbase + i2));
	}

	XMVECTOR normal = g_XMIdentityR1;
	XMVECTOR textureScale = g_XMNegativeOneHalf;

	if (!isTop)
	{
		normal = -normal;
		textureScale *= g_XMNegateX;
	}
	// vertices.
	for (size_t i = 0; i < tessellation; i++)
	{
		XMVECTOR circleVector = GetCircleVector(i, tessellation);

		XMVECTOR position = (circleVector * size.x/2) + (normal * size.y);

		XMVECTOR textureCoordinate = XMVectorMultiplyAdd(XMVectorSwizzle<0, 2, 3, 3>(circleVector), textureScale, g_XMOneHalf);

		ref_vertices.push_back(Vertex::Vertex_UV_Normal_Color(position, textureCoordinate, normal));
	}
}

void ButiEngine::MeshHelper::CreateCone(Vector3 size, int tessellation, const std::vector<Color>& arg_colors, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData)
{
	size.y /= 2;

	XMVECTOR topOffset = g_XMIdentityR1 * size.y;

	float radius = size.x / 2;
	size_t stride = tessellation + 1;

	for (size_t i = 0; i <= tessellation; i++)
	{
		XMVECTOR circlevec = GetCircleVector(i, tessellation);

		XMVECTOR sideOffset = circlevec * radius;

		float u = (float)i / tessellation;

		XMVECTOR textureCoordinate = XMLoadFloat(&u);

		XMVECTOR pt = sideOffset - topOffset;

		XMVECTOR normal = XMVector3Cross(GetCircleTangent(i, tessellation), topOffset - pt);
		normal = XMVector3Normalize(normal);
		outputMeshData.vertices .push_back(Vertex::Vertex_UV_Normal_Color(topOffset, Vector2(0,0), normal));
		outputMeshData.vertices.push_back(Vertex::Vertex_UV_Normal_Color(pt, normal, textureCoordinate + g_XMIdentityR1));
		
		outputMeshData.indices.push_back((UINT)(i * 2));
		outputMeshData.indices.push_back((UINT)((i * 2 + 3) % (stride * 2)));
		outputMeshData.indices.push_back((UINT)((i * 2 + 1) % (stride * 2)));
	}

	CreateCylinderCap(arg_colors, outputMeshData.vertices, outputMeshData.indices, size, tessellation, false);
	//RHからLHに変更
	ReverseWinding(outputMeshData.vertices,outputMeshData.indices);

	VertexFlatConverter(outputMeshData.vertices);
	VertexAttachColor(arg_colors,outputMeshData.vertices);

}

void ButiEngine::MeshHelper::CreateCapsule(Vector3 size, Vector3 pointA, Vector3 pointB, int tessellation, bool isLie, const std::vector<Color>& arg_colors, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData)
{
	if (tessellation < 3) {
		CreateCube(size, arg_colors, outputMeshData,true);
		return;
	}
	size = size * 0.5f;



	outputMeshData.eightCorner.up_left_back = Vector3(-size.x, size.y, -size.z)+pointA;
	outputMeshData.eightCorner.up_left_front = Vector3(-size.x, size.y, size.z) + pointA;
	outputMeshData.eightCorner.up_right_back = Vector3(size.x, size.y, -size.z) + pointA;
	outputMeshData.eightCorner.up_right_front = Vector3(size.x, size.y, size.z) + pointA;

	outputMeshData.eightCorner.down_left_back = Vector3(-size.x, -size.y, -size.z) + pointB;
	outputMeshData.eightCorner.down_left_front = Vector3(-size.x, -size.y, size.z) + pointB;
	outputMeshData.eightCorner.down_right_back = Vector3(size.x, -size.y, -size.z) + pointB;
	outputMeshData.eightCorner.down_right_front = Vector3(size.x, -size.y, size.z) + pointB;

	std::vector< Vertex::Vertex_UV_Normal_Color> vertices;
	std::vector<UINT> indices;

	size_t verticalSegments = tessellation;
	size_t horizontalSegments = tessellation * 2;
	for (size_t i = 0; i <= verticalSegments; i++)
	{


		float v = 1 - (float)i / verticalSegments;

		float latitude = (i * DirectX::XM_PI / verticalSegments) - DirectX::XM_PIDIV2;
		float dy, dxz;

		DirectX::XMScalarSinCos(&dy, &dxz, latitude);

		Vector3 CenterPos = pointA;
		if (i >= (verticalSegments / 2)) {
			CenterPos = pointB;
		}

		float SphereTotalFront = size.x * DirectX::XM_PI / 2.0f;
		float SylinderTotalFront = abs(pointA.y - pointB.y);
		float SphereRate = SphereTotalFront / (SphereTotalFront + SylinderTotalFront) / 2.0f;

		if (v <= 0.5f) {
			v *= SphereRate;
		}
		else {
			v = 1.0f - ((1.0f - v) * SphereRate);
		}


		for (size_t j = 0; j <= horizontalSegments; j++)
		{
			float u = (float)j / horizontalSegments;

			float longitude = j * DirectX::XM_2PI / horizontalSegments;
			float dx, dz;

			DirectX::XMScalarSinCos(&dx, &dz, longitude);

			dx *= dxz;
			dz *= dxz;

			DirectX::XMVECTOR normal = DirectX::XMVectorSet(dx, dy, dz, 0);
			auto pos = Vector3(dx * size.x + CenterPos.x, dy * size.x + CenterPos.y, dz * size.x + CenterPos.z);
			vertices.push_back(Vertex::Vertex_UV_Normal_Color(pos, Vector2(u, v),pos.GetNormalize()));
		}
	}
	size_t stride = horizontalSegments + 1;
	for (size_t i = 0; i < verticalSegments; i++)
	{
		for (size_t j = 0; j <= horizontalSegments; j++)
		{
			size_t nextI = i + 1;
			size_t nextJ = (j + 1) % stride;

			indices.push_back((UINT)(i * stride + j));
			indices.push_back((UINT)(nextI * stride + j));
			indices.push_back((UINT)(i * stride + nextJ));

			indices.push_back((UINT)(i * stride + nextJ));
			indices.push_back((UINT)(nextI * stride + j));
			indices.push_back((UINT)(nextI * stride + nextJ));
		}
	}
	ReverseWinding(vertices, indices);

	if (isLie) {
		for (auto& v : vertices) {
			float tmp = -v.position.y;
			v.position.y = v.position.x;
			v.position.x = tmp;
			float uvTmp = -v.uv.y;
			v.uv.y = v.uv.x;
			v.uv.x = uvTmp;
		}
	}



	outputMeshData.vertices = vertices;
	outputMeshData.indices = indices;
}

void ButiEngine::MeshHelper::CreatePlane(Vector2 size, float UVMax, const UINT arg_verticalSeparate,  const UINT arg_horizontalSeparate, const std::vector<Color>& arg_colors, bool flat, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData)
{
	BoxSurface surface;
	surface.up = size.y * 0.5f;
	surface.down = -size.y * 0.5f;
	surface.right = size.x * 0.5f;
	surface.right = -size.x * 0.5f;
	surface.front = 0;
	surface.back = 0;


	std::vector<Vertex::Vertex_UV_Normal_Color> vertices;
	float horizontalUnit = size.y / (arg_horizontalSeparate);
	float verticalUnit = size.x / (arg_verticalSeparate);




	outputMeshData.eightCorner.up_left_back = Vector3(-size.x, size.y, 0)*0.5f;
	outputMeshData.eightCorner.up_left_front = Vector3(-size.x, size.y, 0) * 0.5f;
	outputMeshData.eightCorner.up_right_back = Vector3(size.x, size.y, 0) * 0.5f;
	outputMeshData.eightCorner.up_right_front = Vector3(size.x, size.y, 0) * 0.5f;

	outputMeshData.eightCorner.down_left_back = Vector3(-size.x, -size.y, 0) * 0.5f;
	outputMeshData.eightCorner.down_left_front = Vector3(-size.x, -size.y, 0) * 0.5f;
	outputMeshData.eightCorner.down_right_back = Vector3(size.x, -size.y, 0) * 0.5f;
	outputMeshData.eightCorner.down_right_front = Vector3(size.x, -size.y, 0) * 0.5f;

	for (int i = 0; i < arg_horizontalSeparate + 1; i++) {
		for (int j = 0; j < arg_verticalSeparate + 1; j++) {
			vertices.push_back(Vertex::Vertex_UV_Normal_Color(Vector3(verticalUnit * i - size.x / 2, horizontalUnit * j - size.y / 2, 0), Vector2((verticalUnit * i / size.x), (1 - horizontalUnit * j / size.y)) * UVMax, Vector3(0, 0, 1.0f)));
		}
	}

	std::vector<UINT>indices;
	for (int i = 0; i < (arg_horizontalSeparate + 1) * arg_verticalSeparate; i += arg_horizontalSeparate + 1) {
		for (int j = i; j < i + arg_horizontalSeparate; j++) {
			indices.push_back(j + arg_horizontalSeparate + 2);
			indices.push_back(j + arg_horizontalSeparate + 1);
			indices.push_back(j);
			indices.push_back(j + 1);
			indices.push_back(j + arg_horizontalSeparate + 2);
			indices.push_back(j);
		}
	}

	if (flat)
		VertexFlatConverter(vertices);
	VertexAttachColor(arg_colors, vertices);
	outputMeshData.vertices = vertices;
	outputMeshData.indices = indices;
}


void ButiEngine::MeshHelper::CreateLine(Vector2 pointA, Vector2 pointB, float width, const std::vector<Color>& arg_colors, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData)
{
	auto vectorAB = Calculator::Normalize2D(Calculator::Float2Addition(Calculator::Float2Multiplication(pointA, -1), pointB));

	auto range = Calculator::Float2Multiplication(Calculator::Normalize2D(Calculator::Float2Verticalize(vectorAB)), width);

	auto p0 = Calculator::Float2Addition(pointA, Calculator::Float2Multiplication(range, -1));
	auto p1 = Calculator::Float2Addition(pointA, Calculator::Float2Multiplication(range, 1));

	auto p2 = Calculator::Float2Addition(pointB, Calculator::Float2Multiplication(range, -1));
	auto p3 = Calculator::Float2Addition(pointB, Calculator::Float2Multiplication(range, 1));


	std::vector<Vertex::Vertex_UV_Normal_Color> vertices
	{
		Vertex::Vertex_UV_Normal_Color(Vector3(p0.x, p0.y, 0.0f),Vector2(0.0f,0.0f)),
		Vertex::Vertex_UV_Normal_Color(Vector3(p1.x, p1.y, 0.0f),Vector2(1.0f,0.0f)),
		Vertex::Vertex_UV_Normal_Color(Vector3(p2.x, p2.y, 0.0f),Vector2(0.0f,1.0f)),
		Vertex::Vertex_UV_Normal_Color(Vector3(p3.x, p3.y, 0.0f),Vector2(1.0f,1.0f)),
		//BackSide
	};


	std::vector<UINT>indices{
		0,1,2,
		3,2,1,
		2,1,0,
		1,2,3
	};

	VertexFlatConverter(vertices);
	VertexAttachColor(arg_colors, vertices);
	outputMeshData.vertices = vertices;
	outputMeshData.indices = indices;
}

void ButiEngine::MeshHelper::CreateHexergon(Vector2 size, const std::vector<Color>& arg_colors, bool flat, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData)
{
	size.x *= 0.5f;
	size.y *= 0.5f;
	std::vector<Vertex::Vertex_UV_Normal_Color> vertices
	{
		Vertex::Vertex_UV_Normal_Color(Vector3(0, size.y, 0.0f),Vector2(0.0f,0.0f)),//0
		Vertex::Vertex_UV_Normal_Color(Vector3(size.x, size.y*0.5f, 0.0f),Vector2(1.0f,0.0f)),//1
		Vertex::Vertex_UV_Normal_Color(Vector3(-size.x, size.y*0.5f, 0.0f),Vector2(0.0f,1.0f)),//2
		Vertex::Vertex_UV_Normal_Color(Vector3(size.x, -size.y*0.5f, 0.0f),Vector2(1.0f,1.0f)),//3
		Vertex::Vertex_UV_Normal_Color(Vector3(-size.x, -size.y*0.5f, 0.0f),Vector2(0.0f,1.0f)),//4
		Vertex::Vertex_UV_Normal_Color(Vector3(0, -size.y, 0.0f),Vector2(1.0f,1.0f)),//5
	};
	std::vector<UINT>indices{
		0,1,2,
		1,3,2,
		2,3,4,
		5,4,3
	};
	if(flat)
		VertexFlatConverter(vertices);
	VertexAttachColor(arg_colors, vertices);
	outputMeshData.vertices = vertices;
	outputMeshData.indices = indices;
}

void ButiEngine::MeshHelper::CreateFourSidesPirabid(Vector3 size, const std::vector<Color>& arg_colors, bool flat, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData)
{
	size.x *= 0.5f;
	size.y *= 0.5f;
	size.z *= 0.5f;

	std::vector<Vertex::Vertex_UV_Normal_Color>vertices{
		// top
		Vertex::Vertex_UV_Normal_Color(Vector3(0, size.y, 0), Vector2(0.5f, 0.0f)),//0
		// front
		Vertex::Vertex_UV_Normal_Color(Vector3(-size.x, -size.y, -size.z), Vector2(0.0f, 1.0f)),//1
		Vertex::Vertex_UV_Normal_Color(Vector3(size.x, -size.y, -size.z), Vector2(1.0f, 1.0f)),//2

		// back
		Vertex::Vertex_UV_Normal_Color(Vector3(size.x, -size.y, size.z), Vector2(0.0f, 1.0f)),//3
		Vertex::Vertex_UV_Normal_Color(Vector3(-size.x, -size.y, size.z), Vector2(1.0f, 1.0f)),//4

		// left
		Vertex::Vertex_UV_Normal_Color(Vector3(size.x, -size.y, -size.z), Vector2(0.0f, 1.0f)),//5
		Vertex::Vertex_UV_Normal_Color(Vector3(size.x, -size.y, size.z), Vector2(1.0f, 1.0f)),//6

		// right
		Vertex::Vertex_UV_Normal_Color(Vector3(-size.x, -size.y, size.z), Vector2(0.0f, 1.0f)),//7
		Vertex::Vertex_UV_Normal_Color(Vector3(-size.x, -size.y, -size.z), Vector2(1.0f, 1.0f)),//8


		// bottom
		Vertex::Vertex_UV_Normal_Color(Vector3(-size.x, -size.y, -size.z), Vector2(0.0f, 0.0f)),//9
		Vertex::Vertex_UV_Normal_Color(Vector3(size.x, -size.y, -size.z), Vector2(1.0f, 0.0f)),//10
		Vertex::Vertex_UV_Normal_Color(Vector3(-size.x, -size.y, size.z), Vector2(0.0f, 1.0f)),//11
		Vertex::Vertex_UV_Normal_Color(Vector3(size.x, -size.y, size.z), Vector2(1.0f, 1.0f))//12
	};
	std::vector<UINT>indices{
		//front
		0,2,1,
		//back
		0,4,3,
		//left
		0,6,5,
		//right
		0,8,7,
		//bottom
		9,10,11,
		12,11,10,
	};
	if(flat)
	VertexFlatConverter(vertices);
	VertexAttachColor(arg_colors, vertices);
	outputMeshData.vertices = vertices;
	outputMeshData.indices = indices;
}

void ButiEngine::MeshHelper::CreateRegularOctahedron(Vector3 size, const std::vector<Color>& arg_colors, bool flat, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData)
{
	size.x *= 0.5f;
	size.y *= 0.5f;
	size.z *= 0.5f;

	std::vector<Vertex::Vertex_UV_Normal_Color>vertices{

		//top

		// top
		Vertex::Vertex_UV_Normal_Color(Vector3(0, size.y, 0), Vector2(0.5f, 0.0f)),//0
		// front
		Vertex::Vertex_UV_Normal_Color(Vector3(-size.x, 0, -size.z), Vector2(0.0f, 1.0f)),//1
		Vertex::Vertex_UV_Normal_Color(Vector3(size.x, 0, -size.z), Vector2(1.0f, 1.0f)),//2

		// back
		Vertex::Vertex_UV_Normal_Color(Vector3(size.x, 0, size.z), Vector2(0.0f, 1.0f)),//3
		Vertex::Vertex_UV_Normal_Color(Vector3(-size.x, 0, size.z), Vector2(1.0f, 1.0f)),//4

		// left
		Vertex::Vertex_UV_Normal_Color(Vector3(size.x, 0, -size.z), Vector2(0.0f, 1.0f)),//5
		Vertex::Vertex_UV_Normal_Color(Vector3(size.x, 0, size.z), Vector2(1.0f, 1.0f)),//6

		// right
		Vertex::Vertex_UV_Normal_Color(Vector3(-size.x, 0, size.z), Vector2(0.0f, 1.0f)),//7
		Vertex::Vertex_UV_Normal_Color(Vector3(-size.x,0, -size.z), Vector2(1.0f, 1.0f)),//8

		//under
		// front
		Vertex::Vertex_UV_Normal_Color(Vector3(-size.x, 0, -size.z), Vector2(0.0f, 1.0f)),//9
		Vertex::Vertex_UV_Normal_Color(Vector3(size.x, 0, -size.z), Vector2(1.0f, 1.0f)),//10

		// back
		Vertex::Vertex_UV_Normal_Color(Vector3(size.x, 0, size.z), Vector2(0.0f, 1.0f)),//11
		Vertex::Vertex_UV_Normal_Color(Vector3(-size.x,0, size.z), Vector2(1.0f, 1.0f)),//12

		// left
		Vertex::Vertex_UV_Normal_Color(Vector3(size.x, 0, -size.z), Vector2(0.0f, 1.0f)),//13
		Vertex::Vertex_UV_Normal_Color(Vector3(size.x, 0, size.z), Vector2(1.0f, 1.0f)),//14

		// right
		Vertex::Vertex_UV_Normal_Color(Vector3(-size.x,0, size.z), Vector2(0.0f, 1.0f)),//15
		Vertex::Vertex_UV_Normal_Color(Vector3(-size.x,0, -size.z), Vector2(1.0f, 1.0f)),//16
		// bottmTop
		Vertex::Vertex_UV_Normal_Color(Vector3(0, -size.y, 0), Vector2(0.5f, 0.0f)),//17

	};
	std::vector<UINT>indices{
		//front
		0,2,1,
		//back
		0,4,3,
		//left
		0,6,5,
		//right
		0,8,7,
		9,10,17,
		11,12,17,
		13,14,17,
		15,16,17,
	};
	VertexFlatConverter(vertices);
	VertexAttachColor(arg_colors, vertices);
	outputMeshData.vertices = vertices;
	outputMeshData.indices = indices;
}

void ButiEngine::MeshHelper::VertexFlatConverter(std::vector<Vertex::Vertex_UV_Normal_Color>& outputVertices)
{
	for (auto itr = outputVertices.begin(); itr != outputVertices.end(); itr++) {
		itr->normal = itr->position.GetNormalize();
	}
}

void ButiEngine::MeshHelper::VertexAttachColor(const std::vector<Color>& arg_colors, std::vector<Vertex::Vertex_UV_Normal_Color>& arg_vertecies)
{
	if (arg_colors.size() <= 0) {
		return;
	}
	if (arg_colors.size() == arg_vertecies.size()) {
		for (int i = 0; i < arg_vertecies.size(); i++) {
			arg_vertecies.at(i).color = arg_colors.at(i);
		}
		return;
	}
	for (int i = 0; i < arg_vertecies.size(); i++) {
		arg_vertecies.at(i).color = arg_colors.at(0);
	}
	return;
}

void ButiEngine::MeshHelper::CreateCirclePolygon(const float radius, const UINT tessellation, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData)
{
	outputMeshData.vertices.clear();
	outputMeshData.indices.clear();
	if (tessellation < 3) {
		CreateCube(Vector3(radius, radius, radius), std::vector<Vector4>{}, outputMeshData, true);
		return;
	}
	std::vector< Vertex::Vertex_UV_Normal_Color> vertices;
	std::vector<UINT> indices;
	std::vector<UINT> remainVertexIndex;
	std::vector<UINT> buffer;
	Vector3 normal = Vector3(0, 0, 1.0f);

	vertices.reserve(tessellation);
	indices.reserve((size_t)(tessellation - 2) * 3);
	remainVertexIndex.reserve(tessellation);
	buffer.reserve(tessellation);

	for (int i = 0; i < tessellation; i++) {
		float x = 0, y = 0;

		DirectX::XMScalarSinCos(&x, &y, DirectX::XM_2PI / tessellation * i);
		auto  vertex= Vertex::Vertex_UV_Normal_Color();

		vertex.position = Vector3(radius*x,radius*y,0);
		vertex.uv = Vector2((1+x)/2,1- (1+y)/2);
		vertex.normal = normal;


		vertices.push_back(vertex);
		
		remainVertexIndex.push_back(i);
	}
	int offset = 0;
	do
	{
		offset = 0;
		for (; offset < remainVertexIndex.size();) {
			buffer.push_back(remainVertexIndex.at(offset));
			if ((offset + 2) > (remainVertexIndex.size() )) {
				break;
			}

			indices.push_back(remainVertexIndex.at(offset));
			indices.push_back(remainVertexIndex.at(offset+1));
			if((offset + 2)>remainVertexIndex.size()-1)
				indices.push_back(remainVertexIndex.at(0));
			else {
				indices.push_back(remainVertexIndex.at(offset + 2));
			}

			offset += 2;
		}
		remainVertexIndex = buffer;
		buffer.clear();
		buffer.reserve(remainVertexIndex.size());
	} while (remainVertexIndex.size()>=3);

	BoxSurface surface;
	surface.up = radius;
	surface.down = -radius;
	surface.right = radius;
	surface.left = -radius;
	surface.front = 0;
	surface.back = 0;

	outputMeshData.vertices = vertices;
	outputMeshData.indices=indices ;
}

void ButiEngine::MeshHelper::CreateImmediateMeshForParticle(const UINT arg_particleCount, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData)
{
	outputMeshData.Clear();
	outputMeshData.vertices.reserve(arg_particleCount);
	outputMeshData.indices.reserve(arg_particleCount);
	auto vertex = Vertex::Vertex_UV_Normal_Color();
	vertex.position = Vector3(10000000000000000, 10000000000000000, 1000000000000000);
	for (unsigned int i = 0; i < arg_particleCount; i++) {
		outputMeshData.vertices.push_back(vertex);
		outputMeshData.indices.push_back(i);
	}
}



