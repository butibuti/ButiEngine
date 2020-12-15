#pragma once
#include"../Common/ID.h"
namespace ButiEngine {
	class GameObject;
	using GameObjectTag= ID<GameObject>;

	class GameObjectTagManager
	{
	public:
		GameObjectTag CreateGameObjectTag(std::string arg_tagName);
		GameObjectTag GetObjectTag(std::string arg_tagName);
		std::string GetTagName(const GameObjectTag& arg_gameObj);
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(idManager);
		}
	private:
		IDManager<GameObject> idManager;
	};

	void OutputCereal(const std::shared_ptr<GameObjectTagManager>& v, const std::string& arg_filePath);

	void InputCereal(std::shared_ptr<GameObjectTagManager>& v,const std::string& arg_filePath );
}