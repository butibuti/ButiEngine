#pragma once

#include"stdafx.h"
#include"Header/Device/Helper/ShaderHelper.h"
#include<filesystem>

int main() {

	std::cout << "Dx11のシェーダコンパイルなら0、Dx12なら1を入力してください" << std::endl;
	int compileVer = 0;

	std::cin >> compileVer;
	
	ButiEngine::ShaderHelper::CompileType compileType = (ButiEngine::ShaderHelper::CompileType)compileVer;

	if (compileVer > 1 || compileVer < 0) {
		std::cout << "対応していないバージョンです" << std::endl;
		std::system("pause");
		return 0;
	}

	std::cout << "コンパイルしたいファイルがあるディレクトリのパスを入力してください" << std::endl;
	std::string compileTargetDirectory;
	std::cin >>compileTargetDirectory ;

	std::vector< std::string> vec_filePathes;

	std::filesystem::directory_iterator itr(compileTargetDirectory),end;

	std::error_code err;

	for (; itr != end && !err; itr.increment(err)) {
		const std::filesystem::directory_entry entry = *itr;
		std::cout << entry.path().string()<<"をコンパイルします"<< std::endl;
		vec_filePathes.push_back( entry.path().string());

	}
	
	int isLoop = 1;

	while (isLoop)
	{

		for (auto itr = vec_filePathes.begin(); itr != vec_filePathes.end(); itr++) {
			ButiEngine::ShaderHelper::Compile(*itr, compileType);
		}
		std::cout << "0で終了、1で継続" << std::endl;
		std::cin >> isLoop;
	}


	std::system("pause");
	return 0;
}
