
useColor()

function wireframe(x0, y0, z0, x1, y1, z1)
	for x = x0, x1, 1 do
		setVoxel(x, y0, z0, 1)
		setVoxel(x, y1, z0, 1)
		setVoxel(x, y1, z1, 1)
		setVoxel(x, y0, z1, 1)
	end
	
	for y = y0, y1, 1 do
		setVoxel(x0, y, z0, 1)
		setVoxel(x1, y, z0, 1)
		setVoxel(x1, y, z1, 1)
		setVoxel(x0, y, z1, 1)
	end
	
	for z = z0, z1, 1 do
		setVoxel(x0, y0, z, 1)
		setVoxel(x1, y0, z, 1)
		setVoxel(x1, y1, z, 1)
		setVoxel(x0, y1, z, 1)
	end
end

function line(x0, y0, z0, x1, y1, z1, count)
	for i = 0, count, 1 do
		setVoxel(
			x0 + x1 * i,
			y0 + y1 * i,
			z0 + z1 * i,
			1)
	end
end

half = {}
half.x = size.width / 2
half.y = size.height / 2
half.z = size.depth / 2

wireframe(
	0,
	0,
	0,
	size.width - 1,
	size.height - 1,
	size.depth - 1)
	
wireframe(
	half.x / 2,
	half.y / 2,
	half.z / 2,
	half.x * 3 / 2 - 1,
	half.y * 3 / 2 - 1,
	half.z * 3 / 2 - 1)

line(0, 				0, 					0,				 1,  1,  1, half.x / 2)
line(size.width - 1,	0, 					0,				-1,  1,  1, half.x / 2)
line(size.width - 1,	size.height - 1,	0,				-1, -1,  1, half.x / 2)
line(0, 				size.height - 1,	0,				 1, -1,  1, half.x / 2)
line(0, 				0, 					size.depth - 1,  1,  1, -1, half.x / 2)
line(size.width - 1,	0, 					size.depth - 1, -1,  1, -1, half.x / 2)
line(size.width - 1,	size.height - 1,	size.depth - 1, -1, -1, -1, half.x / 2)
line(0, 				size.height - 1,	size.depth - 1,  1, -1, -1, half.x / 2)
