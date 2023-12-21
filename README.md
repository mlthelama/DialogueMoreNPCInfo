# DialogueMoreNPCInfo
![Platform](https://img.shields.io/static/v1?label=platform&message=windows&color=dimgray&style=for-the-badge&logo=windows)
![GitHub release (latest by date)](https://img.shields.io/github/v/release/mlthelama/DialogueMoreNPCInfo?style=for-the-badge)
![GitHub](https://img.shields.io/github/license/mlthelama/DialogueMoreNPCInfo?style=for-the-badge)
![GitHub top language](https://img.shields.io/github/languages/top/mlthelama/DialogueMoreNPCInfo?style=for-the-badge)
![GitHub language count](https://img.shields.io/github/languages/count/mlthelama/DialogueMoreNPCInfo?style=for-the-badge)
![GitHub last commit](https://img.shields.io/github/last-commit/mlthelama/DialogueMoreNPCInfo?style=for-the-badge)
![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/mlthelama/DialogueMoreNPCInfo/main.yml?style=for-the-badge)

## Nexus Mods
* [DialogueMoreNPCInfo](https://www.nexusmods.com/skyrimspecialedition/mods/71866)

## End User Dependencies
* [SKSE64](https://skse.silverlock.org/)
* [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444)
* [Scaleform Translation Plus Plus](https://www.nexusmods.com/skyrimspecialedition/mods/22603)
* [Icons for Dialog More NPC Info](https://www.nexusmods.com/skyrimspecialedition/mods/71868)
* [Auto Input Switch](https://www.nexusmods.com/skyrimspecialedition/mods/54309)

## Build Dependencies
* [CommonLibSSE NG](https://github.com/CharmedBaryon/CommonLibSSE-NG)
* [spdlog](https://github.com/gabime/spdlog)
* [simpleini](https://github.com/brofield/simpleini)
* [vcpkg](https://github.com/microsoft/vcpkg) 
  - Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
* [cmake](https://cmake.org) 
  - installed dir needs to be added to the `PATH` environment variable
  - on hash `93895b28ea7bc8cda10f156c5d336f3fc070f8b1`

## Building
```
git clone https://github.com/mlthelama/DialogueMoreNPCInfo.git
cd DialogueMoreNPCInfo

cmake --preset vs2022-windows
cmake --build --preset vs2022-windows --config Release
```
