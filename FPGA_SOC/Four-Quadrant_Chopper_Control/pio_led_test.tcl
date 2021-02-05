get_service_paths master
set master_service_path [ lindex [get_service_paths master] 0]
open_service master $master_service_path

master_write_8 $master_service_path 0x00010000 0x01
after 1000
master_write_8 $master_service_path 0x00010000 0x02
after 1000
master_write_8 $master_service_path 0x00010000 0x04
after 1000
master_write_8 $master_service_path 0x00010000 0x08
after 1000
master_write_8 $master_service_path 0x00010000 0x10
after 1000
master_write_8 $master_service_path 0x00010000 0x20
after 1000
master_write_8 $master_service_path 0x00010000 0x40
after 1000
master_write_8 $master_service_path 0x00010000 0x80
after 1000
close_service master $master_service_path
