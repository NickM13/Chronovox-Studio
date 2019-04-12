
--[[
local function writeln(str)
	file:write(str .. "\n")
end

local function addV(x, y, z)
	writeln("v " .. x .. " " .. y .. " " .. z)
end

local function addVN(x, y, z)
	writeln("")
end

local function addF(x, y, z)
	writeln("")
end
--]]

--file = io.open("fileboi.obj", "w")

--writeln("# " .. app.getAppName .. " v" .. app.getAppVersion .. " OBJ File")
--writeln("# www.nickvoxel.com")

--file:close()