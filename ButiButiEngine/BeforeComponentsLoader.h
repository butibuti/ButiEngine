#pragma once
namespace ButiEngine {
	class GameComponent;
	class Behavior; 
	class Application;
	class BeforeComponentsLoader
	
	{
	public:
		static void BeforeBehaviorLoad(std::vector<std::shared_ptr<Behavior>>& vec_shp_addBehavior);
		static void BeforeComponentsLoad(std::vector<std::shared_ptr<GameComponent>>&vec_shp_addComponents,std::shared_ptr<Application>& arg_app);

	private:
	};
}

