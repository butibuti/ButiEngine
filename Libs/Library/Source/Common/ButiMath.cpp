#include "stdafx.h"
#include "..\..\Header\Common\ButiMath.h"
#include "..\..\Header\Common\CerealUtill.h"

void ButiEngine::OutputCereal(const Vector2& v, const std::string& path)
{
	std::stringstream stream;


	cereal::BinaryOutputArchive binOutArchive(stream);

	binOutArchive(v);

	std::ofstream outputFile(path, std::ios::out);

	outputFile << stream.str();

	outputFile.close();
	stream.clear();
}

void ButiEngine::InputCereal(Vector2& v, const std::string& path)
{
	std::stringstream stream;

	std::ifstream inputFile(path, std::ios::in);

	stream << inputFile.rdbuf();

	cereal::BinaryInputArchive binInputArchive(stream);


	binInputArchive(v);
}

void ButiEngine::OutputCereal(const Vector3& v, const std::string& path)
{

	std::stringstream stream;


	cereal::BinaryOutputArchive binOutArchive(stream);

	binOutArchive(v);

	std::ofstream outputFile(path, std::ios::out);

	outputFile << stream.str();

	outputFile.close();
	stream.clear();
}

void ButiEngine::InputCereal( Vector3& v, const std::string& path)
{

	std::stringstream stream;

	std::ifstream inputFile(path, std::ios::in);

	stream << inputFile.rdbuf();

	cereal::BinaryInputArchive binInputArchive(stream);


	binInputArchive(v);
}

void ButiEngine::OutputCereal(const Vector4& v, const std::string& path)
{
	std::stringstream stream;


	cereal::BinaryOutputArchive binOutArchive(stream);

	binOutArchive(v);

	std::ofstream outputFile(path, std::ios::out);

	outputFile << stream.str();

	outputFile.close();
	stream.clear();
}

void ButiEngine::InputCereal(Vector4& v, const std::string& path)
{
	std::stringstream stream;

	std::ifstream inputFile(path, std::ios::in);

	stream << inputFile.rdbuf();

	cereal::BinaryInputArchive binInputArchive(stream);


	binInputArchive(v);
}

void ButiEngine::OutputCereal(const Matrix4x4& v, const std::string& path)
{
	std::stringstream stream;


	cereal::BinaryOutputArchive binOutArchive(stream);

	binOutArchive(v);

	std::ofstream outputFile(path, std::ios::out);

	outputFile << stream.str();

	outputFile.close();
	stream.clear();
}

void ButiEngine::InputCereal(Matrix4x4& v, const std::string& path)
{
	std::stringstream stream;

	std::ifstream inputFile(path, std::ios::in);

	stream << inputFile.rdbuf();

	cereal::BinaryInputArchive binInputArchive(stream);


	binInputArchive(v);
}

void ButiEngine::OutputCereal(const Line& v, const std::string& path)
{
	std::stringstream stream;


	cereal::BinaryOutputArchive binOutArchive(stream);

	binOutArchive(v);

	std::ofstream outputFile(path, std::ios::out);

	outputFile << stream.str();

	outputFile.close();
	stream.clear();
}

void ButiEngine::InputCereal(Line& v, const std::string& path)
{
	std::stringstream stream;

	std::ifstream inputFile(path, std::ios::in);

	stream << inputFile.rdbuf();

	cereal::BinaryInputArchive binInputArchive(stream);


	binInputArchive(v);
}

bool ButiEngine::SplineCurve::ShowUI()
{
	bool ret = false;
	if (GUI::TreeNode("SplineCurve Editor")) {

		GUI::BulletText("StartPoint");

		if (GUI::DragFloat3("##startdrag", &vec_points[0].x, 0.01f, -500, 500)) {
			vec_points[1] = vec_points[0];
			ret = true;
		}
		GUI::BulletText("ControllPoints");
		for (int i = 2; i < vec_points.size() - 2; i++) {
			if (GUI::DragFloat3(("##controlldrag" + std::to_string(i)).c_str(), &vec_points[i].x, 0.01f, -500, 500))
				ret = true;
			GUI::SameLine();

			if (GUI::Button(("Insert##" + std::to_string(i)).c_str())) {
				auto itr = vec_points.begin();
				itr += i;
				vec_points.insert(itr, Vector3(vec_points[i]));
				Initialize();
				ret = true;
			}
			if (vec_points.size() > 6) {
				GUI::SameLine();
				if (GUI::Button(("Delete##" + std::to_string(i)).c_str())) {
					auto itr = vec_points.begin();
					itr += i;
					vec_points.erase(itr);
					i--;
					Initialize();
					ret = true;
				}
			}

		}


		GUI::BulletText("EndPoint");


		if (GUI::DragFloat3("##enddrag", &vec_points[vec_points.size() - 2].x, 0.01f, -500, 500)) {
			vec_points[vec_points.size() - 1] = vec_points[vec_points.size() - 2]; ret = true;
		}

		if (GUI::Button("Loop")) {
			vec_points[0] = vec_points[vec_points.size() - 2];
			vec_points[1] = vec_points[vec_points.size() - 2];
			ret = true;
		}

		GUI::TreePop();

	}
	return ret;
}

char ButiEngine::MathHelper::GetByteSize(const int arg_check)
{
		if (arg_check <= CharMax) {
			return 1;
		}
		else if (arg_check <= ShortMax) {
			return 2;
		}
		else if (arg_check <= IntMax) {
			return 4;
		}
}

char ButiEngine::MathHelper::GetUnsignedByteSize(const UINT arg_check)
{
	if (arg_check <= CharMax * 2) {
		return 1;
	}
	else if (arg_check <= ShortMax * 2) {
		return 2;
	}
	else if (arg_check <= IntMax * 2) {
		return 4;
	}
}
