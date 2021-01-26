#include"stdafx.h"
#include"../Resources/ModelAnimation.h"

namespace ButiEngine {
	class Resource_Motion:public IResource_Motion {
	public:
		void Initialize() {}
		void PreInitialize(){}
		std::shared_ptr<ModelAnimation> GetAnimation()override;
		void AddKeyFrame(const std::string& arg_boneName, const MotionKeyFrameData& arg_data)override;
		void AddKeyFrameLane(const std::string& arg_boneName,const std::vector<MotionKeyFrameData>& arg_datas)override;
	private:
		std::unordered_map<std::string, std::vector<MotionKeyFrameData>> map_boneKeyFrames;
	};

}