#pragma once

#define GFX_EXCEPT() Graphics::GraphicsException(__LINE__, __FILE__, hr)
#define GFX_EXCEPT_THROW(code) if(FAILED(hr = code)){throw GFX_EXCEPT();}
#define GFX_LAST_EXCEPT() Graphics::GraphicsException(__LINE__, __FILE__, GetLastError())
#define GFX_LAST_EXCEPT_THROW(code) if(FAILED(code)){throw GFX_LAST_EXCEPT();}