#include "..\..\Header\Graphics\Layer.h"

GE::Layer::Layer(IRenderTexture* newRenderTexture, IDepthTexture* newDepthTexture)
	: renderTexture(newRenderTexture)
	, depthTexture(newDepthTexture)
	, dontDeleteRenderTexture(false)
	, dontDeleteDepthTexture(false)
{
}

GE::Layer::~Layer()
{
	if (dontDeleteRenderTexture == false)delete renderTexture;
	if (dontDeleteDepthTexture == false)delete depthTexture;
}

void GE::Layer::Create(IRenderTexture* newRenderTexture, IDepthTexture* newDepthTexture)
{
	renderTexture = newRenderTexture;
	depthTexture = newDepthTexture;
}

void GE::Layer::SetDontDeleteTextureFlag(bool renderTexture, bool depthTexture)
{
	dontDeleteRenderTexture = renderTexture;
	dontDeleteDepthTexture = depthTexture;
}

GE::IRenderTexture* GE::Layer::GetRenderTexture()
{
	return renderTexture;
}

GE::IDepthTexture* GE::Layer::GetDepthTexture()
{
	return depthTexture;
}
