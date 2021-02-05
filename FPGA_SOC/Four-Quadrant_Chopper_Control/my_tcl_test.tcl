get_service_paths master
set master_service_path [ lindex [get_service_paths master] 0]
open_service master $master_service_path

master_write_16 $master_service_path 0x00030000 0x0004
after 1000
set reply0 [master_read_16 $master_service_path 0x00030000 1]
format "%x" $reply0
puts "data from state register: $reply0"
after 1000

master_write_16 $master_service_path 0x00030002 0x0FA0
after 1000
set reply1 [master_read_16 $master_service_path 0x00030002 1]
format "%x" $reply1
puts "data from periode register:$reply1"
after 1000

master_write_16 $master_service_path 0x00030004 0x0064
after 1000
set reply2 [master_read_16 $master_service_path 0x00030004 1]
format "%x" $reply2
puts "data from duty cycle register:$reply2"
after 1000

master_write_16 $master_service_path 0x00030006 0x000A
after 1000
set reply3 [master_read_16 $master_service_path 0x00030006 1]
format "%x" $reply3
puts "data from dead time register: $reply3"
after 1000

close_service master $master_service_path
