
color.r = 255
color.g = 255
color.b = 255

xadd  = 2.0 / size.width
yadd  = 2.0 / size.height
zadd  = 2.0 / size.depth

xcircle = -1.0 + xadd / 2
ycircle = -1.0 + yadd / 2
zcircle = -1.0 + zadd / 2

useColor()

for x = 0.0, size.width, 1.0 do
	ycircle = -1.0 + yadd / 2
	for y = 0.0, size.height, 1.0 do
		zcircle = -1.0 + zadd / 2
		for z = 0.0, size.depth, 1.0 do
			if math.sqrt(math.pow(xcircle, 2) + math.pow(ycircle, 2) + math.pow(zcircle, 2)) <= 1.0 then
				setVoxel(x, y, z, 1)
			end
			zcircle = zcircle + zadd
		end
		ycircle = ycircle + yadd
	end
	xcircle = xcircle + xadd
end
