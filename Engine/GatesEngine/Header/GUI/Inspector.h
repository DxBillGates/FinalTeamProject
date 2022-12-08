#pragma once
#include "..\GameFramework\GameObject\GameObject.h"

namespace GE
{
	namespace GUI
	{
		class Inspector
		{
		private:
			GameObject* currentSelectGameObject;
		public:
			void SetCurrentSelectGameObject(GameObject* gameObject);
			GameObject* CurrentSelectGameObject();
			void OnGui();
		};
	}
}