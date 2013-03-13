#include "StdAfx.h"
#include "MyGameTextBase.h"
#include <iostream>
#include <exception>
#include <string>
using std::exception;
using std::runtime_error;
using std::cout;
using std::wstring;
#include "MyGame3DDevice.h"

#define _CRT_SECURE_NO_WARNINGS
LPDIRECT3DDEVICE9 MyGameTextBase::pDevice = 0;

int						MyGameTextBase::screenWidth = 0;
int						MyGameTextBase::screenHeight =0;

MyGameTextBase::MyGameTextBase( float _height, float _width, int _weight, string _fontName, bool _italic )
	:font(0),destroyed(false),
	visible(true),
	count(20),format( DT_TOP|DT_LEFT ), color( 0xff000000 ),pDestRect( 0 )
{
	if( !screenWidth )
	{
		screenWidth = MyGame3DDevice::GetWidth();
		screenHeight = MyGame3DDevice::GetHeight();
	}
	ZeroMemory( &df, sizeof( D3DXFONT_DESC ) );
	df.Height   = _height*( (float)screenHeight / 768.0 );
	df.Width	= _width*( (float)screenWidth / 1024.0 );
	//df.Height   = _height;
	//df.Width	= _width;
	df.Weight	= _weight;
	df.MipLevels= D3DX_DEFAULT;
	df.Italic	= _italic;
	df.CharSet	= DEFAULT_CHARSET;
	df.OutputPrecision = 0;
	df.Quality	= 0;
	df.PitchAndFamily = MONO_FONT | FF_MODERN;
	//setlocale(LC_ALL, "zh_CN");
	setlocale(LC_ALL, "");
	const char* _text = "微软雅黑";
	//TODO: 这里注释了一行代码
	mbstowcs( df.FaceName, _text, _fontName.size()+1 );
	//size_t convertedChars;
	//mbstowcs_s( &convertedChars, df.FaceName, _fontName.size()+1, _text,  _TRUNCATE);
	HR(D3DXCreateFontIndirect(
		pDevice,
		&df,
		&font ))

	pDestRect = new RECT;
	pDestRect->top = 0;
	pDestRect->left = 0;
	pDestRect->right = 0;
	pDestRect->bottom = 0;
	textToDraw[0] = L'\0';
}


MyGameTextBase::~MyGameTextBase(void)
{
	if(false == destroyed)
		DestroyText();

	delete pDestRect;
}

void MyGameTextBase::DestroyText()
{
	font->Release();
	font = 0;
	destroyed = true;
}

void MyGameTextBase::DrawUIText( wchar_t* _text, int _count, LPRECT _rect, DWORD _format, D3DCOLOR _color )
{
	if( _count == 0 )
		return;
	try{
		//int errorCode = this->font->DrawTextA( 0, _text, _count, &_rect, _format, _color );
		//wchar_t _textWide[500];
		static char* a = setlocale(LC_ALL, "zh_CN");
		//mbstowcs( _textWide, _text, strlen( _text ) ); 
		int errorCode = this->font->DrawTextW( 0, _text, _count, _rect, _format, _color );
		if (errorCode ==0 )
			throw runtime_error( "DrawText-Error,errorcode:"+ errorCode );
	}catch( runtime_error _err){
		cout<<"MyGameUISprite.cpp"<<_err.what()<<std::endl;
	}
}

void MyGameTextBase::DrawUIText( const char* _text, int _count, LPRECT _rect, DWORD _format, D3DCOLOR _color )
{
	if( _count == 0 )
		return;
	try{
		//int errorCode = this->font->DrawTextA( 0, _text, _count, &_rect, _format, _color );
		//wchar_t _textWide[500];
		static char* a = setlocale(LC_ALL, "");
		//mbstowcs( _textWide, _text, strlen( _text ) ); 
		int errorCode = this->font->DrawTextA( 0, _text, _count, _rect, _format, _color );
		if (errorCode ==0 )
			throw runtime_error( "DrawText-Error,errorcode:"+ errorCode );
	}catch( runtime_error _err){
		cout<<"MyGameUISprite.cpp"<<_err.what()<<std::endl;
	}
}

void MyGameTextBase::SetFormat( DWORD _format )
{
	this->format = _format;
}

void MyGameTextBase::SetDestRectForScreen( int _x, int _y, int _width, int _height )
{
	this->pDestRect->left	= _x*(screenWidth/1024.0f);
	this->pDestRect->top	= _y*(screenHeight/768.0f);
	this->pDestRect->right	= ( _x + _width )*(screenWidth/1024.0f);
	this->pDestRect->bottom = ( _y + _height )*(screenHeight/768.0f);
}

void MyGameTextBase::SetDestRectForTexture( int _x, int _y, int _width, int _height )
{
	this->pDestRect->left	= _x;
	this->pDestRect->top	= _y;
	this->pDestRect->right	= ( _x + _width );
	this->pDestRect->bottom = ( _y + _height );
}

void myWstrcpy( wchar_t* _dest, const wchar_t* _src )
{
	if( _dest == _src )
		return;
	unsigned int len = wcslen( _src );
	memcpy( _dest, _src, sizeof(wchar_t)*(len+1));
}

int MyGameTextBase::SetText( const char* _text )
{
	//text = _text;
	//wchar_t _textWide[500];
	static char* tmp = setlocale(LC_ALL, "zh_CN");
	//TODO: 这里囧了一行代码
	mbstowcs( textToDraw, _text, strlen( _text )+1 );
//	size_t convertedChars;
//	mbstowcs_s( &convertedChars, textToDraw, strlen( _text )+1, _text,  _TRUNCATE);
	unsigned int len = wcslen( textToDraw );
	//static wstring text_w;
	if( len >= 50 )
		MyGameMessage("the string to render is too long");
	return len;
}

void MyGameTextBase::SetCount(int _count)
{
	this->count = _count;
}

void MyGameTextBase::SetColor( D3DCOLOR _color )
{
	this->color =_color;
}

void MyGameTextBase::RenderText()
{
	this->DrawUIText( textToDraw,
						count, 
						pDestRect,
						format,
						color );
}