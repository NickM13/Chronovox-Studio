
file = io.open("fileboi.obj", "w")

local function writeln(str)
	file:write(str .. "\n")
end

local function addV(x, y, z, w)
	writeln("v " .. x .. " " .. y .. " " .. z .. " " .. w)
end

writeln("# " .. app.getAppName() .. " v" .. app.getAppVersion() .. " OBJ File")
writeln("# www.nickvoxel.com")



file:close()