#pragma once

#include "BaseManager.h"
#include <string>
#include <d2d1.h>
#include <DirectXMath.h>

using namespace DirectX;

class TextUtil
{
public:
	static std::wstring ConvertToWideString(const std::string& inputText);

	static XMFLOAT2 GetTextSize(const std::wstring& text, IDWriteTextFormat* textFormat);
};
