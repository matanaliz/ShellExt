set currPath=%~dp0
taskkill /f /im explorer.exe
explorer %currPath%
exit