#pragma once
#include "ILayer.h"

namespace GE
{
	class Layer : public ILayer
	{
	private:
		IRenderTexture* renderTexture;
		IDepthTexture* depthTexture;
		bool dontDeleteRenderTexture;
		bool dontDeleteDepthTexture;
	public:
		Layer(IRenderTexture* newRenderTexture = nullptr, IDepthTexture* newDepthTexture = nullptr);
		~Layer();
		void Create(IRenderTexture* newRenderTexture, IDepthTexture* newDepthTexture);
		void SetDontDeleteTextureFlag(bool renderTexture, bool depthTexture);

		// interface

		IRenderTexture* GetRenderTexture() override;
		IDepthTexture* GetDepthTexture() override;
	};
}
