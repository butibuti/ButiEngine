#include"stdafx.h"
#include "..\..\Header\Common\Transform.h"
#include "..\..\Header\Common\CerealUtill.h"

void ButiEngine::OutputCereal(std::shared_ptr<Transform>& v, const std::string& path)
{
	std::stringstream stream;


	cereal::BinaryOutputArchive binOutArchive(stream);

	binOutArchive(v);

	std::ofstream outputFile(path, std::ios::out);

	outputFile << stream.str();

	outputFile.close();
	stream.clear();
}

void ButiEngine::InputCereal(std::shared_ptr<Transform>& v, const std::string& path)
{
	std::stringstream stream;

	std::ifstream inputFile(path, std::ios::in);

	stream << inputFile.rdbuf();

	cereal::BinaryInputArchive binInputArchive(stream);


	binInputArchive(v);
}

bool ButiEngine::Transform::ShowUI()
{
	bool isEdit = false;
	GUI::BulletText("Position");
	if (GUI::DragFloat3("##p", &localPosition.x, 0.02f, -FLT_MAX, FLT_MAX, "%.3f")) {
		localMatrix = nullptr;
		isEdit = true;
	}
	GUI::BulletText("Scale");
	if (GUI::DragFloat3("##s", &scale.x, 0.01f, -FLT_MAX, FLT_MAX, "%.3f")) {
		localMatrix = nullptr;
		isEdit = true;
	}
	GUI::BulletText("Rotation");
	static Vector3 euler;
	(GUI::DragFloat3("##R", euler, 0.01, -360, 360, "%.3f"));
	if (GUI::Button("Roll")) {

		RollWorldRotation(euler);
		isEdit = true;
	}
	GUI::SameLine();
	if (GUI::Button("SetRotation")) {
		RollIdentity();
		RollWorldRotation(euler);
		isEdit = true;
	}

	GUI::PushButtonRepeat(true);

	if (GUI::Button("X:+")) {
		RollLocalRotationX_Degrees(1);
		isEdit = true;
	}GUI::SameLine();

	if (GUI::Button("Y:+")) {
		RollLocalRotationY_Degrees(1);
		isEdit = true;
	}GUI::SameLine();

	if (GUI::Button("Z:+")) {
		RollLocalRotationZ_Degrees(1);
		isEdit = true;
	}
	if (GUI::Button("X:-")) {
		RollLocalRotationX_Degrees(-1);
		isEdit = true;
	}GUI::SameLine();

	if (GUI::Button("Y:-")) {
		RollLocalRotationY_Degrees(-1);
		isEdit = true;
	}GUI::SameLine();

	if (GUI::Button("Z:-")) {
		RollLocalRotationZ_Degrees(-1);
		isEdit = true;
	}

	GUI::PushButtonRepeat(false);

	GUI::BulletText("Look");

	static Vector3 lookTarget;

	GUI::DragFloat3("##lookPosition", lookTarget, 0.01, -FLT_MAX, FLT_MAX);

	GUI::SameLine();
	if (GUI::Button("Look")) {
		SetLookAtRotation(lookTarget);
		lookTarget = Vector3();
		isEdit = true;
	}

	if (GUI::Button("Identity"))
	{
		RollIdentity();
		isEdit = true;
	}

	return isEdit;
}

bool ButiEngine::BoneTransform::ShowUI()
{
	bool isEdit = false;
	if (baseTransform) {

		GUI::BulletText("Position");
		if (GUI::DragFloat3("##p", &localPosition.x, 0.02f, -FLT_MAX, FLT_MAX, "%.3f")) {
			localMatrix = nullptr;
			isEdit = true;
		}
		GUI::BulletText("Scale");
		if (GUI::DragFloat3("##s", &scale.x, 0.01f, -FLT_MAX, FLT_MAX, "%.3f")) {
			localMatrix = nullptr;
			isEdit = true;
		}
	}
	GUI::BulletText("Rotation");
	static Vector3 euler;
	(GUI::DragFloat3("##R", euler, 0.01, -360, 360, "%.3f"));
	if (GUI::Button("Roll")) {

		RollWorldRotation(euler);
		isEdit = true;
	}
	GUI::SameLine();
	if (GUI::Button("SetRotation")) {
		RollIdentity();
		RollWorldRotation(euler);
		isEdit = true;
	}

	GUI::PushButtonRepeat(true);

	if (GUI::Button("X:+")) {
		RollLocalRotationX_Degrees(1);
		isEdit = true;
	}GUI::SameLine();

	if (GUI::Button("Y:+")) {
		RollLocalRotationY_Degrees(1);
		isEdit = true;
	}GUI::SameLine();

	if (GUI::Button("Z:+")) {
		RollLocalRotationZ_Degrees(1);
		isEdit = true;
	}
	if (GUI::Button("X:-")) {
		RollLocalRotationX_Degrees(-1);
		isEdit = true;
	}GUI::SameLine();

	if (GUI::Button("Y:-")) {
		RollLocalRotationY_Degrees(-1);
		isEdit = true;
	}GUI::SameLine();

	if (GUI::Button("Z:-")) {
		RollLocalRotationZ_Degrees(-1);
		isEdit = true;
	}

	GUI::PushButtonRepeat(false);

	GUI::BulletText("Look");

	static Vector3 lookTarget;

	GUI::DragFloat3("##lookPosition", lookTarget, 0.01, -FLT_MAX, FLT_MAX);

	GUI::SameLine();
	if (GUI::Button("Look")) {
		SetLookAtRotation(lookTarget);
		lookTarget = Vector3();
		isEdit = true;
	}

	if (GUI::Button("Identity"))
	{
		RollIdentity();
		isEdit = true;
	}
	return isEdit;
}
