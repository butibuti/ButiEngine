#pragma once
#include "../Device/ObjectFactory.h"


namespace ButiEngine {

	class ICBuffer :public IObject {
	public:

		void SetSlot(const UINT arg_slot) {
			slot = arg_slot;
		}
		const UINT GetSlot()const {
			return slot;
		}
		const std::string& GetExName()const {
			return exName;
		}
		void SetExName(const std::string& arg_cbufferName) {
			exName = arg_cbufferName;
		}
		virtual void Attach(const UINT slotOffset) const = 0;

		virtual void Update()const {
		}

		virtual void CreateBuffer(const bool arg_mapKeep = true) = 0;
		virtual void SetGraphicDevice(std::shared_ptr<GraphicDevice> arg_graphicDevice) = 0;

		void ShowUI();
		

		virtual bool OnShowUI() { return false; }

		virtual std::shared_ptr<ICBuffer> Clone()=0;

		UINT slot = 0;
		std::string exName;
	protected:
	};

	template <class T>
	class CBuffer:public ICBuffer
	{
	public:
		CBuffer() {}
		~CBuffer() {}
		virtual T& Get() const = 0;
	};

}
