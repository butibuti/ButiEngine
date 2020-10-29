#pragma once

#include"stdafx.h"
#include"Header/Device/Helper/ShaderHelper.h"
#include<filesystem>

int main() {

	std::cout << "Dx11�̃V�F�[�_�R���p�C���Ȃ�0�ADx12�Ȃ�1����͂��Ă�������" << std::endl;
	int compileVer = 0;

	std::cin >> compileVer;
	
	ButiEngine::ShaderHelper::CompileType compileType = (ButiEngine::ShaderHelper::CompileType)compileVer;

	if (compileVer > 1 || compileVer < 0) {
		std::cout << "�Ή����Ă��Ȃ��o�[�W�����ł�" << std::endl;
		std::system("pause");
		return 0;
	}

	std::cout << "�R���p�C���������t�@�C��������f�B���N�g���̃p�X����͂��Ă�������" << std::endl;
	std::string compileTargetDirectory;
	std::cin >>compileTargetDirectory ;

	std::vector< std::string> vec_filePathes;

	std::filesystem::directory_iterator itr(compileTargetDirectory),end;

	std::error_code err;

	for (; itr != end && !err; itr.increment(err)) {
		const std::filesystem::directory_entry entry = *itr;
		std::cout << entry.path().string()<<"���R���p�C�����܂�"<< std::endl;
		vec_filePathes.push_back( entry.path().string());

	}
	
	int isLoop = 1;

	while (isLoop)
	{

		for (auto itr = vec_filePathes.begin(); itr != vec_filePathes.end(); itr++) {
			ButiEngine::ShaderHelper::Compile(*itr, compileType);
		}
		std::cout << "0�ŏI���A1�Ōp��" << std::endl;
		std::cin >> isLoop;
	}


	std::system("pause");
	return 0;
}
