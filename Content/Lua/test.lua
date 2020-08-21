function call_me_from_unreal(number)
    location = get_player_location()
    print(location.x, location.y, location.z)
--    return "string"
    return number * 2 + location.x + location.y + location.z
end

current_time = os.date()
--return "TEST"
print("Hello, i am a lua script, current time is: " .. current_time)