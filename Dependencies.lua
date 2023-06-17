
-- NodeEditor Dependencies

IncludeDir = {}
IncludeDir["stb_image"] = "%{wks.location}/NodeEditor/vendor/stb_image"
IncludeDir["GLFW"] = "%{wks.location}/NodeEditor/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/NodeEditor/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/NodeEditor/vendor/ImGui"
IncludeDir["glm"] = "%{wks.location}/NodeEditor/vendor/glm"

LibraryDir = {}

Library = {}

-- Windows
Library["WinSock"] = "Ws2_32.lib"
Library["WinMM"] = "Winmm.lib"
Library["WinVersion"] = "Version.lib"
Library["BCrypt"] = "Bcrypt.lib"