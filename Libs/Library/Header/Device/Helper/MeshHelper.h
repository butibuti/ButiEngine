#include<vector>
#include"../../Resources/Vertex.h"
namespace ButiEngine {
	class MeshHelper
	{
	public:
		~MeshHelper();
		static void ReverseWinding(std::vector<Vertex::Vertex_UV_Normal_Color>& ref_vertices, std::vector<UINT>& ref_indices);
		static DirectX::XMVECTOR GetCircleTangent(int i, int tessellation);
		static DirectX::XMVECTOR GetCircleVector(int size, int tessellation);
		
		//
		static void CreateTriangle(Vector3 point1, Vector3 point2, Vector3 point3, const std::vector<Color>& arg_colors, bool flat, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData);
		
		static void CreateSameTextureCube(Vector3 size, const std::vector<Color>& arg_colors,bool flat, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData);
		
		static void CreateCube(Vector3 size, const std::vector<Color>& arg_colors, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData,bool flat);
		
		static void CreateSphere(Vector3 size, int tessellation, const std::vector<Color>& arg_colors, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData);
		
		static void CreateSphereForParticle(Vector3 size, int tessellation, const std::vector<Color>& arg_colors, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData);
		
		static void CreateCylinderCap(const std::vector<Color>& arg_colors,std::vector<Vertex::Vertex_UV_Normal_Color>& ref_vertices, std::vector<UINT>& ref_indices, Vector3 size, int tessellation, bool isTop);
		
		static void CreateCone(Vector3 size, int tessellation, const std::vector<Color>& arg_colors, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData);
		
		static void CreateCapsule(Vector3 size, Vector3 pointA, Vector3 pointB, int tessellation, bool isLie, const std::vector<Color>& arg_colors, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData);
				
		static void CreatePlane(Vector2 size, Vector3 offset,float tilt,  float UVMax, const UINT arg_verticalSeparate, const UINT arg_horizontalSeparate, const std::vector<Color>& arg_colors, bool flat, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData);
		
		static void CreateHexergon(Vector2 size, const std::vector<Color>& arg_colors, bool flat, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData);
		
		static void CreateFourSidesPirabid(Vector3 size, const std::vector<Color>& arg_colors, bool flat, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData);
		
		static void CreateRegularOctahedron(Vector3 size, const std::vector<Color>& arg_colors, bool flat, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData);
		
		static void VertexFlatConverter(std::vector<Vertex::Vertex_UV_Normal_Color>& outputVertecies);

		static void VertexAttachColor(const std::vector<Color>& arg_colors,std::vector<Vertex::Vertex_UV_Normal_Color>& arg_vertecies);

		static void CreateCirclePolygon(const float radius,const UINT	tessellation, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData);

		static void CreateCameraFrustum(const float angle, const float width,const float height,const float nearclip, const float farclip, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData);
		
		static void CreateImmediateMeshForParticle(const UINT arg_particleCount, BackupData<Vertex::Vertex_UV_Normal_Color>& outputMeshData);
	private:
		MeshHelper();
	};
}

