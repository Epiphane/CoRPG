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

