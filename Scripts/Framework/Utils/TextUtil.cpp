#include "TextUtil.h"
#include "D3DMgr.h"
#include <windows.h>
#include <stdexcept>

std::wstring TextUtil::ConvertToWideString(const std::string& inputText)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, inputText.c_str(), -1, nullptr, 0);

	std::wstring wideString(size_needed, 0);

	int chars_converted = MultiByteToWideChar(CP_UTF8, 0, inputText.c_str(), -1, &wideString[0], size_needed);

	wideString.resize(chars_converted - 1);

	return wideString;
}

XMFLOAT2 TextUtil::GetTextSize(const std::wstring& text, IDWriteTextFormat* textFormat)
{
	XMFLOAT2 size{ 0.0f, 0.0f };

	if (!textFormat || text.empty()) return size;

	IDWriteTextLayout* textLayout = nullptr;

	HRESULT hr = D3DMgr::instance.dwriteFactory->CreateTextLayout(
		text.c_str(),
		static_cast<UINT32>(text.length()),
		textFormat,
		1000.0f,
		1000.0f,
		&textLayout
	);

	if (FAILED(hr) || !textLayout) return size;

	DWRITE_TEXT_METRICS textMetrics{};
	textLayout->GetMetrics(&textMetrics);

	size.x = textMetrics.width;
	size.y = textMetrics.height;

	textLayout->Release();

	return size;
}