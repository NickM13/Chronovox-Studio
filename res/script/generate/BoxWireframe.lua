
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

wireframe(
	0,
	0,
	0,
	size.width - 1,
	size.height - 1,
	size.depth - 1)
