px4_add_module(
	MODULE modules__param_bridge
	MAIN param_bridge
	SRCS
		param_bridge.cpp
	DEPENDS
		px4_work_queue
	)
