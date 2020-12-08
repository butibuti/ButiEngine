#pragma once
#include"stdafx.h"

#include"../Resources/Resource_Material.h"
#include"../Resources/Resource_Model.h"
#include"../Resources/Resource_Mesh.h"
#include"../Resources/Resource_Shader.h"
#include"../Resources/Resource_Texture.h"
#include"../Resources/Resource_Sound.h"
#include"../Resources/Resource_Motion.h"
namespace ButiEngine {
	class ResourceFactory :public IObject {
	protected:
		struct TextureResourceData {
			UINT width;
			UINT height;
			std::vector<BYTE> rawData;
		};
	public:
		virtual std::shared_ptr<Resource_Texture> CreateTextureFromFile(const std::string& filePath) = 0;
		TextureResourceData GetTextureResourceDataFromFile(const std::string& filePath);
		std::shared_ptr<Resource_Material> CreateMaterial(const std::string& arg_filePath, const std::string& arg_fileDirectory = "");
		virtual std::shared_ptr<Resource_Material> CreateMaterial(const MaterialVariable& arg_materialVar, const TextureTag& arg_textureTag) = 0;
		virtual std::shared_ptr<Resource_Material> CreateMaterial(const MaterialVariable& arg_materialVar, const std::vector< TextureTag>& arg_textureTag) = 0;
		virtual std::shared_ptr<Resource_PixelShader> CreatePixelShaderFromFile(const std::string& filePath) = 0;
		virtual std::shared_ptr<Resource_VertexShader> CreateVertexShaderFromFile(const std::string& filePath) = 0;
		virtual std::shared_ptr<Resource_GeometryShader> CreateGeometryShaderFromFile(const std::string& filePath) = 0;
		void Initialize()override {};
		void PreInitialize()override {};
		std::shared_ptr<Resource_Model>CreateModel(const std::string& modelPath, const std::string& fileDirectory = "");
		std::shared_ptr<Resource_Motion>CreateMotion(const std::string& modelPath, const std::string& fileDirectory = "");
	protected:
		std::weak_ptr<GraphicDevice> wkp_graphicDevice;
	private:
		void ReadIndex(std::vector<UINT>& indices, char arg_indexByteCount, BinaryReader& arg_reader);
		template<typename T>
		void ReadPosition(T& out, BinaryReader& arg_reader) {
			out.position = arg_reader.ReadVariable<Vector3>();
		}
		template<typename T>
		void inline ReadVertex(std::vector< T>& out, const int vertexCount, BinaryReader& arg_reader, BoxSurface& boxSurface) {
			float up = 0, down = 0, left = 0, right = 0, front = 0, back = 0;
			out.reserve(vertexCount);
			for (UINT i = 0; i < vertexCount; i++) {
				auto type = arg_reader.ReadVariable<char>();
				auto vertex = arg_reader.ReadVariable<T>();
				if (up < vertex.position.y) {
					up = vertex.position.y;
				}
				else if (down > vertex.position.y) {
					down = vertex.position.y;
				}
				if (right < vertex.position.x) {
					right = vertex.position.x;
				}
				else if (left > vertex.position.x) {
					left = vertex.position.x;
				}
				if (front < vertex.position.z) {
					front = vertex.position.z;
				}
				else if (back > vertex.position.z) {
					back = vertex.position.z;
				}

				out.push_back(vertex);
			}
			boxSurface.up = up;
			boxSurface.down = down;
			boxSurface.left = left;
			boxSurface.right = right;
			boxSurface.back = back;
			boxSurface.front = front;
		}
		template<typename T>
		void ReadNormal(T& out, BinaryReader& arg_reader) {
			out.SetNormal(arg_reader.ReadVariable<Vector3>());

		}
		template<typename T>
		void ReadUV(T& out, BinaryReader& arg_reader) {
			out.SetUV(arg_reader.ReadVariable<Vector2>());
		}
		template<typename T>
		void ReadExUV(T& out, const int uvExCount, BinaryReader& arg_reader) {
			if (uvExCount > 0) {

				out.SetUV_ex1(arg_reader.ReadVariable<Vector2>());
			}
			if (uvExCount > 1) {

				out.SetUV_ex2(arg_reader.ReadVariable<Vector2>());
			}
			if (uvExCount > 2) {
				out.SetUV_ex3(arg_reader.ReadVariable<Vector2>());
			}
			if (uvExCount > 3) {
				out.SetUV_ex4(arg_reader.ReadVariable<Vector2>());
			}
		}
	};
}