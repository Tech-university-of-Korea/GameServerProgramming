#include "pch.h"
#include "ClientNetwork.h"
#include "Graphic.h"

Graphic* graphic;
std::unique_ptr<ClientNetwork> gClientNetwork = std::make_unique<ClientNetwork>();