del "*.sln" /s /f /q
@RD /S /Q ".idea"
@RD /S /Q ".vs"
@RD /S /Q "Binaries"
@RD /S /Q "Build"
@RD /S /Q "Intermediate"
@RD /S /Q "DerivedDataCache"
@RD /S /Q "Saved/Autosaves"
@RD /S /Q "Saved/Crashes"
@RD /S /Q "Saved/Logs"
@RD /S /Q "Saved/Config/CrashReportClient"
@RD /S /Q "Saved/Config/LevelState"
@RD /S /Q "Saved/Config/WorldState"

for /f "delims=" %%b in ('dir /b /ad Plugins') do rmdir /s /q "Plugins\%%b\Intermediate"
for /f "delims=" %%b in ('dir /b /ad Plugins') do rmdir /s /q "Plugins\%%b\Binaries"

set /p DUMMY=Hit ENTER to continue...