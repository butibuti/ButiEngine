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

void ButiEngine::Transform::ShowUI()
{
	GUI::BulletText("Position");
	if (GUI::DragFloat3("##p", &localPosition.x, 0.02f, -500.0f, 500.0f, "%.3f")) {
		localMatrix = nullptr;
	}
	GUI::BulletText("Scale");
	if (GUI::DragFloat3("##s", &scale.x, 0.01f, -500.0, 500.0f, "%.3f")) {
		localMatrix = nullptr;
	}
	GUI::BulletText("Rotation");
	static Vector3 euler;
	(GUI::DragFloat3("##R", euler, 0.01, -360, 360, "%.3f"));
	if (GUI::Button("Roll")) {

		RollWorldRotation(euler);
	}
	GUI::SameLine();
	if (GUI::Button("SetRotation")) {
		RollIdentity();
		RollWorldRotation(euler);
	}

	GUI::PushButtonRepeat(true);

	if (GUI::Button("X:+")) {
		RollLocalRotationX_Degrees(1);
	}GUI::SameLine();

	if (GUI::Button("Y:+")) {
		RollLocalRotationY_Degrees(1);
	}GUI::SameLine();

	if (GUI::Button("Z:+")) {
		RollLocalRotationZ_Degrees(1);
	}
	if (GUI::Button("X:-")) {
		RollLocalRotationX_Degrees(-1);
	}GUI::SameLine();

	if (GUI::Button("Y:-")) {
		RollLocalRotationY_Degrees(-1);
	}GUI::SameLine();

	if (GUI::Button("Z:-")) {
		RollLocalRotationZ_Degrees(-1);
	}

	GUI::PushButtonRepeat(false);

	if (GUI::Button("Identity"))
	{
		RollIdentity();
	}
}
