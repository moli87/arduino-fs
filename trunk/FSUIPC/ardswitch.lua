foot =3.28083989501312
speed = 115200
handshake = 0
serial_wait = 100
inited = 0
states = {}
dev = com.open("COM9", speed, handshake)
if dev == 0 then
    ipc.display("Could not open device port")
    ipc.exit()
end


flaps_inc = ipc.readSW(0x3bfa)
ipc.log("Flaps inc = ".. flaps_inc)


-- index from 1
commands={}
table.insert(commands, {0x0bc8, 0x0bc8, "parkingb", ipc.writeUW, ipc.readUW, 32767, 0})
table.insert(commands, {0x0d0c, 0x0d0c, "lights", ipc.writeUW, ipc.readUW, -1, -1})
table.insert(commands, {0x07bc, 0x07bc, "master", ipc.writeUD, ipc.readUD, 1, 0})
table.insert(commands, {0x07c0, 0x07c0, "winglevel", ipc.writeUD, ipc.readUD, 1, 0})
table.insert(commands, {0x07c8, 0x07c8, "hhold", ipc.writeUD, ipc.readUD, 1, 0})
table.insert(commands, {0x07d0, 0x07d0, "ahold", ipc.writeUD, ipc.readUD, 1, 0})
table.insert(commands, {0x07c4, 0x07c4, "navhold", ipc.writeUD, ipc.readUD, 1, 0})
table.insert(commands, {0x07ec, 0x07ec, "vshold", ipc.writeUD, ipc.readUD, 1, 0})
table.insert(commands, {0x0800, 0x0800, "approachhold", ipc.writeUD, ipc.readUD, 1, 0})
table.insert(commands, {0x07dc, 0x07dc, "speedhold", ipc.writeUD, ipc.readUD, 1, 0})
table.insert(commands, {0x0810, 0x0810, "throttle", ipc.writeUD, ipc.readUD, 1, 0})
table.insert(commands, {0x0892, 0x0892, "magneto", ipc.writeUD, ipc.readUD, -1, -1})
table.insert(commands, {0x281c, 0x281c, "battery", ipc.writeUW, ipc.readUW, 1, 0})
table.insert(commands, {0x2e80, 0x2e80, "avionics", ipc.writeUW, ipc.readUW, 1, 0})
table.insert(commands, {0x3101, 0x3101, "alternator", ipc.writeUB, ipc.readUB, 1, 0})
table.insert(commands, {0x3104, 0x3104, "fuel", ipc.writeUB, ipc.readUB, 1, 0})
table.insert(commands, {0x337c, 0x337c, "deice", ipc.writeUB, ipc.readUB, 1, 0})
table.insert(commands, {0x337d, 0x337d, "deice", ipc.writeUB, ipc.readUB, 1, 0})
table.insert(commands, {0x029c, 0x029c, "pitot", ipc.writeUB, ipc.readUB, 1, 0})

table.insert(commands, {66079, 66080, "gear", ipc.control, ipc.readUD, 0, 0})

function do_action (name, action)
    for i,v in ipairs(commands) do
        if string.match (name,v[3]) then
            ipc.log(v[2])
            if string.match (action, "on") then
                v[4](v[1],v[6])
                states [v[1]] =v[6]
            elseif string.match (action, "off") then
                v[4](v[2],v[7])
                states [v[2]] =v[7]
            elseif string.match (action, "toggle") then
                current_value =v[5](v[1])
                if current_value  ~=v[6] then
                    v[4](v[1],v[6])
                elseif current_value  ~=v[6] then
                    v[4](v[2],v[7])
                end
            else
                v[4](v[1], tonumber (action))
                states [v[1]] = tonumber (action)
            end
            break
        end
    end
end

function call_offset (offset, value)
    for i,v in ipairs(commands) do
        if offset ==v[1] then
            if states[offset] and value ~= states[offset] and inited==1 then
                v[4](offset, states[offset])
            end
        end
        break
    end
end



function handlecom(handle, str)
    ipc.log("com=" .. str)
    command_name, command_value = string.match (str, "(%a+):(.*)")
    if command_name then
        do_action (command_name, command_value)

        if string.match(command_name, "init") then
	    if string.match(command_value, "on") then
	        inited=1
            else
	        inited=0
            end
	end
    end	
    if string.match(str, "nav1s:(%d+)") then
    	state = "0x"..string.match(str, "nav1s:(%d+)")
        ipc.writeUW(0x311e, state)
    end
    if string.match(str, "nav2s:(%d+)") then
    	state = "0x"..string.match(str, "nav2s:(%d+)")
        ipc.writeUW(0x3120, state)
    end

if string.match(str, "com1s:(%d+)") then
    	state = "0x"..string.match(str, "com1s:(%d+)")
        ipc.writeUW(0x311a, state)
    end
if string.match(str, "com2s:(%d+)") then
    	state = "0x"..string.match(str, "com2s:(%d+)")
        ipc.writeUW(0x311c, state)
    end

    if string.match(str, "adf1:(%d+)") then
    	state = "0x"..string.match(str, "adf1:(%d+)")
        ipc.writeUW(0x034c, state)
    end
    if string.match(str, "adf2:(%d+)") then
    	state = "0x"..string.match(str, "adf2:(%d+)")
        ipc.writeUW(0x02d4, state)
    end
    if string.match(str, "trans:(%d+)") then
    	state = "0x"..string.match(str, "trans:(%d+)")
        ipc.writeUW(0x0354, state)
    end

    if string.match(str, "flaps:(-?%d+)") then
    	state = tonumber(string.match(str, "flaps:(-?%d+)"))
        ipc.control(66534, state)
    end
if string.match(str, "ailtrim:(-?%d+)") then
    	state = tonumber(string.match(str, "ailtrim:(-?%d+)"))
        ipc.writeSW(0x0c02, state)
    end
if string.match(str, "ruddertrim:(-?%d+)") then
    	state = tonumber(string.match(str, "ruddertrim:(-?%d+)"))
        ipc.writeSW(0x0c04, state)
    end

    if string.match(str, "spoilers:(-?%d+)") then
    	state = tonumber(string.match(str, "spoilers:(-?%d+)"))
        ipc.control(66382, state)
    end
    if string.match(str, "altitude:(-?%d+)") then
    	state = tonumber(string.match(str, "altitude:(-?%d+)"))
        current =  ipc.readUD(0x07d4) + ((state*100*65536)/foot)
        ipc.writeUD(0x07d4, current )
    end
    if string.match(str, "obs2:(-?%d+)") then
    	state = tonumber(string.match(str, "obs2:(-?%d+)"))
        current =  ipc.readUW(0xc5e) + state
        ipc.writeUW(0x0c5e, current )
    end
    if string.match(str, "obs1:(-?%d+)") then
    	state = tonumber(string.match(str, "obs1:(-?%d+)"))
        current =  ipc.readUW(0x0c4e) + state
        if current >359 then
            current = current -360
        end
        if current <0 then
           current = current +360
        end 
        ipc.writeUW(0x0c4e, current )
    end

    if string.match(str, "heading:(-?%d+)") then
    	state = tonumber(string.match(str, "heading:(-?%d+)"))
        current =  ipc.readUW(0x07cc) + state*65536/360
        ipc.writeUW(0x07cc, current )
    end
    if string.match(str, "vertical:(-?%d)") then
    	state = tonumber(string.match(str, "vertical:(-?%d)"))
        current =  ipc.readSW(0x07f2)+ state*100
        if current > 7000 then
            current = 7000
        elseif current <-7000 then
            current = -7000
        end
	current = current
        ipc.writeSW(0x07f2, current )
    end
    if string.match (str,"swap:(%x)") then
        state =tonumber(string.match(str, "swap:(%x)"))
        ipc.writeUB (0x3123, state)
    end	
end

-- ----------------------------- Callbacks -------------------


function call_radio_DME (offset, value)
    if offset ==0x0300 then
        command = "dme1:" .. string.format("%04d", value) .. "\n"
    elseif offset ==0x0304 then
        command = "dme1time:" .. string.format("%04d", value) .. "\n"
    elseif offset ==0x0306 then
        command = "dme2:" .. string.format("%04d", value) .. "\n"
    elseif offset ==0x030a then
        command = "dme2time:" .. string.format("%04d", value) .. "\n"
    end
    ipc.log ("writes com =".. command)
    com.write (dev, command)
    ipc.sleep(serial_wait)
end


function call_radio_degrees (offset, value)
    if offset ==0x0c4e then
        command = "obs1:" .. string.format("%04d", value) .. "\n"
    elseif offset ==0x0c5e then
        command = "obs2:" .. string.format("%04d", value) .. "\n"
    end
    ipc.log ("writes com =".. command)
    com.write (dev, command)
    ipc.sleep(serial_wait)
end


function call_altitude (offset, value)
    value = (value/65536)* foot/100
    command = "altitude:" .. string.format("%04d", value) .. "\n"
    ipc.log ("writes com =".. command)
    com.write (dev, command)
    ipc.sleep(serial_wait)
end


function call_heading (offset, value)
    value = (value/65536)*360
    command = "heading:" .. string.format("%04d", value) .. "\n"
    ipc.log ("writes com =".. command)
    com.write (dev, command)
    ipc.sleep(serial_wait)
end


function call_vertical (offset, value)
    ipc.log ("writes com = vertical:"..string.format("%04d", value/10).. "\n")
    com.write (dev, "vertical:" .. string.format("%04d", value/10) .. "\n")
    ipc.sleep(serial_wait)
end


function call_radio (offset, value)
    if offset ==0x311e then
        command = "nav1s:" .. string.format("%04x", value) .. "\n"
    elseif offset ==0x034c then
        command = "adf1:" .. string.format("%04x", value) .. "\n"
    elseif offset ==0x02d4 then
        command = "adf2:" .. string.format("%04x", value) .. "\n"
    elseif offset ==0x034e then
        command = "com1m:" .. string.format("%04x", value) .. "\n"
    elseif offset ==0x0350 then
        command = "nav1m:" .. string.format("%04x", value) .. "\n"
    elseif offset ==0x0352 then
        command = "nav2m:" .. string.format("%04x", value) .. "\n"
    elseif offset ==0x0354 then
        command = "trans:" .. string.format("%04x", value) .. "\n"
    elseif offset ==0x3118 then
        command = "com2m:" .. string.format("%04x", value) .. "\n"
    elseif offset ==0x311a then
        command = "com1s:" .. string.format("%04x", value) .. "\n"
    elseif offset ==0x311c then
        command = "com2s:" .. string.format("%04x", value) .. "\n"
    elseif offset ==0x311e then
        command = "nav1s:" .. string.format("%04x", value) .. "\n"
    elseif offset ==0x3120 then
        command = "nav2s:" .. string.format("%04x", value) .. "\n"
    end
    ipc.log ("writes com =".. command)
    com.write (dev, command)
    ipc.sleep(serial_wait)
end


event.com(dev, 20, 5, '\n', "handlecom")
com.write(dev,"resend:1\n")

ipc.sleep(10000)
com.write(dev,"resend:1\n")
event.offset(0x0BC8, "UW", "call_offset") -- parking brake
event.offset(0x0d0c, "UW", "call_offset") -- lights 
event.offset(0x0Be8, "UD", "call_offset") -- gear
event.offset(0x07BC, "UD", "call_offset") -- master ap
event.offset(0x0810, "UD", "call_offset") -- ap at


event.offset (0x02D4, "UW", "call_radio") -- ADF two main
event.offset (0x0300, "UW", "call_radio_DME") -- VOR1 DME distance
event.offset (0x0304, "UW", "call_radio_DME") -- VOR1 DME time
event.offset (0x0306, "UW", "call_radio_DME") -- VOR2 DME distance
event.offset (0x030a, "UW", "call_radio_DME") -- VOR2 DME time
event.offset (0x034c, "UW", "call_radio") -- ADF one frequency main
event.offset (0x034e, "UW", "call_radio") -- com one main
event.offset (0x0350, "UW", "call_radio") -- nav one main
event.offset (0x0352, "UW", "call_radio") -- nav two main
event.offset (0x0354, "UW", "call_radio") -- transponder
event.offset (0x0c4e, "UW", "call_radio_degrees") -- nav one obs
event.offset (0x0c5e, "UW", "call_radio_degrees") -- nav two obs
event.offset (0x3118, "UW", "call_radio") -- com two
event.offset (0x311a, "UW", "call_radio") -- com one standby
event.offset (0x311c, "UW", "call_radio") -- com two standby
event.offset (0x311e, "UW", "call_radio") -- nav 1 standby
event.offset (0x3120, "UW", "call_radio") -- nav two standby
event.offset (0x07cc, "UW", "call_heading") -- autopilot heading
event.offset (0x07d4, "UD", "call_altitude") -- autopilot altitude
event.offset (0x07f2, "SW", "call_vertical") -- autopilot vertical speed