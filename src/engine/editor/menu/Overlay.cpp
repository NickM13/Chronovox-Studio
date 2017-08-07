#include "engine\editor\menu\Overlay.h"

Model* Overlay::m_model = 0;
Container* Overlay::m_container = 0;

Sint32 *Overlay::r = new Sint32(), *Overlay::g = new Sint32(255), *Overlay::b = new Sint32();
Sint32 *Overlay::hue = new Sint32(), *Overlay::sat = new Sint32(), *Overlay::val = new Sint32();
Sint32 *Overlay::w = new Sint32(), *Overlay::h = new Sint32(), *Overlay::d = new Sint32();
Sint32 *Overlay::x = new Sint32(), *Overlay::y = new Sint32(), *Overlay::z = new Sint32();
Uint16 *Overlay::toolbar = new Uint16(), *Overlay::toolbarMeta = new Uint16();
