#pragma once
#include"stdafx.h"
#include"CBuffer.h"
namespace ButiEngine {
	template <class T>
	class CArrayBuffer:public ICBuffer
	{
	public:
		virtual T& Get(const UINT arg_index)  = 0;
	protected:
	};

}