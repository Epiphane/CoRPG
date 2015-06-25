Input = {
	done = false,
	val = ""
}
function Input:new(o)
	o = o or {}
	o.done = false
	o.val = ""
	setmetatable(o, self)
	self.__index = self
	return o
end

function Input:update(input)
	if not self.done then
		if (input == '\b') then
			if (string.len(self.val) > 0) then
				self.val = string.sub(self.val, 1, string.len(self.val) - 1)
			end
		elseif (input == '\n') then
			self.done = true
		else
			self.val = self.val..input
		end
	end
end

NumInput = {
	done = false,
	val = 0
}
function NumInput:new(o)
	o = o or {}
	o.done = false
	o.val = 0
	setmetatable(o, self)
	self.__index = self
	return o
end

function NumInput:update(input)
	if not self.done then
		if (input == '\b') then
			self.val = math.floor(self.val / 10)
		elseif (input == '\n') then
			self.done = true
		elseif (input >= '0' and input <= '9') then
			self.val = math.floor(self.val * 10 + (input - '0'))
		end
	end
end
