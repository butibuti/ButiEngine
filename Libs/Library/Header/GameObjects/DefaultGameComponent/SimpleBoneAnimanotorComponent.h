#pragma once
#include"../GameComponent.h"

#include"Header//Resources/DrawData/IDrawData.h"
namespace ButiEngine {
	class ModelAnimation;

	class ModelDrawData;
	class SimpleBoneAnimatorComponent:public GameComponent {
	public:
		SimpleBoneAnimatorComponent(std::shared_ptr< ModelDrawData > arg_shp_modelData);
		void OnUpdate()override;
		void AddAnimation(std::shared_ptr<ModelAnimation> arg_shp_animation); 
		void AddAnimation(MotionTag arg_motionTag);
		void SetLoop(const bool arg_isLoop);
		std::string GetGameComponentName()override {
			return "SimpleBoneAnimatorComponent";
		}
	private:
		std::shared_ptr<ModelAnimation> shp_animation;
		std::shared_ptr< ModelDrawData > shp_modelData;
		bool isMotionActive=true;
	};

}