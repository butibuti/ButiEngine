#pragma once

namespace ButiEngine {

	struct SceneInformation :public IObject
	{
	public:
		SceneInformation(const std::string& arg_sceneName) {
			sceneName = arg_sceneName;
		}
		~SceneInformation();
		std::string GetSceneName() {
			return sceneName;
		}
		void Initialize()override {}
		void PreInitialize()override {}
	private:
		std::string sceneName;
	};
	struct SceneChangeInformation :public IObject
	{
	public:
		SceneChangeInformation() {
		}
		void Initialize()override {}
		void PreInitialize()override {}
	private:
	};
	struct SceneRenderingInformation 
	{
	public:
		SceneRenderingInformation() {
		}

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(vec_cameraTransform);
			archive(vec_cameraProperty);
			archive(layerCount);
		}
		std::vector<std::shared_ptr<Transform>> vec_cameraTransform;
		std::vector<CameraProjProperty> vec_cameraProperty;
		int layerCount = 1;
	private:
	};

	void OutputCereal(const std::shared_ptr<SceneRenderingInformation>& v, std::string arg_path);

	void InputCereal(std::shared_ptr<SceneRenderingInformation>& v,std::string arg_path);


}

