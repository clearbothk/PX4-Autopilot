#include <drivers/drv_hrt.h>
#include <parameters/param.h>
#include <px4_platform_common/defines.h>
#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <px4_platform_common/px4_work_queue/ScheduledWorkItem.hpp>
#include <string.h>
#include <uORB/Publication.hpp>
#include <uORB/Subscription.hpp>
#include <uORB/topics/param_bridge_request.h>
#include <uORB/topics/param_bridge_response.h>

using namespace time_literals;

class ParamBridge : public ModuleBase<ParamBridge>,
                    public ModuleParams,
                    public px4::ScheduledWorkItem {
public:
  ParamBridge()
      : ModuleParams(nullptr),
        ScheduledWorkItem(MODULE_NAME, px4::wq_configurations::lp_default) {}
  ~ParamBridge() { ScheduleClear(); }

  static int task_spawn(int argc, char *argv[]);
  static int custom_command(int argc, char *argv[]);
  static int print_usage(const char *reason = nullptr);

  bool init();

private:
  void Run() override;

  void handle_get_request(param_bridge_request_s *req, param_t *p,
                          param_bridge_response_s *res);
  void handle_set_request(param_bridge_request_s *req, param_t *p,
                          param_bridge_response_s *res);

  uORB::Subscription _request_sub{ORB_ID(param_bridge_request)};
  uORB::Publication<param_bridge_response_s> _response_pub{
      ORB_ID(param_bridge_response)};
};

bool ParamBridge::init() {
  ScheduleOnInterval(10_ms);
  return true;
}

void ParamBridge::handle_get_request(param_bridge_request_s *req, param_t *p,
                                     param_bridge_response_s *res) {
  switch (param_type(*p)) {
  case PARAM_TYPE_INT32:
    param_get(*p, &(res->int_value));
    res->param_type = param_bridge_response_s::TYPE_INT;
    res->status = param_bridge_response_s::STATUS_OK;
    break;

  case PARAM_TYPE_FLOAT:
    param_get(*p, &(res->float_value));
    res->param_type = param_bridge_response_s::TYPE_FLOAT;
    res->status = param_bridge_response_s::STATUS_OK;
    break;

  default:
    PX4_ERR("Unsupported param type for GET: %d on param %.16s", param_type(*p),
            req->param_name);
    res->status = param_bridge_response_s::STATUS_ERROR;
  }
}

void ParamBridge::handle_set_request(param_bridge_request_s *req, param_t *p,
                                     param_bridge_response_s *res) {
  switch (param_type(*p)) {
  case PARAM_TYPE_INT32:
    if (param_set(*p, &(req->int_value)) == 0) {
      param_save_default(false);
      res->int_value = req->int_value;
      res->param_type = param_bridge_response_s::TYPE_INT;
      res->status = param_bridge_response_s::STATUS_OK;
    } else {
      PX4_ERR("SET %.16s failed", req->param_name);
      res->status = param_bridge_response_s::STATUS_ERROR;
    }
    break;

  case PARAM_TYPE_FLOAT:
    if (param_set(*p, &(req->float_value)) == 0) {
      param_save_default(false);
      res->float_value = req->float_value;
      res->param_type = param_bridge_response_s::TYPE_FLOAT;
      res->status = param_bridge_response_s::STATUS_OK;
    } else {
      PX4_ERR("SET %.16s failed", req->param_name);
      res->status = param_bridge_response_s::STATUS_ERROR;
    }
    break;

  default:
    PX4_ERR("Unsupported param type for SET: %d on param %.16s", param_type(*p),
            req->param_name);
    res->status = param_bridge_response_s::STATUS_ERROR;
  }
}

void ParamBridge::Run() {
  if (should_exit()) {
    ScheduleClear();
    exit_and_cleanup();
    return;
  }

  param_bridge_request_s req;

  while (_request_sub.update(&req)) {
    param_bridge_response_s res{};
    res.timestamp = hrt_absolute_time();
    memcpy(res.param_name, req.param_name, sizeof(res.param_name));
    res.param_name[sizeof(res.param_name) - 1] = '\0';

    param_t p = param_find_no_notification(req.param_name);

    if (p == PARAM_INVALID) {
      PX4_WARN("param not found: %.16s", req.param_name);
      res.status = param_bridge_response_s::STATUS_NOT_FOUND;
    } else if (req.operation == param_bridge_request_s::OP_GET) {
      handle_get_request(&req, &p, &res);
    } else if (req.operation == param_bridge_request_s::OP_SET) {
      handle_set_request(&req, &p, &res);
    } else {
      res.status = param_bridge_response_s::STATUS_ERROR;
    }

    _response_pub.publish(res);
  }
}

int ParamBridge::task_spawn(int argc, char *argv[]) {
  ParamBridge *instance = new ParamBridge();

  if (instance) {
    _object.store(instance);
    _task_id = task_id_is_work_queue;

    if (instance->init()) {
      return PX4_OK;
    }

  } else {
    PX4_ERR("alloc failed");
  }

  delete instance;
  _object.store(nullptr);
  _task_id = -1;

  return PX4_ERROR;
}

int ParamBridge::custom_command(int argc, char *argv[]) {
  return print_usage("unknown command");
}

int ParamBridge::print_usage(const char *reason) {
  if (reason) {
    PX4_WARN("%s\n", reason);
  }

  PRINT_MODULE_DESCRIPTION(
      "Parameter bridge: GET/SET PX4 params by name over uXRCE-DDS");
  PRINT_MODULE_USAGE_NAME("param_bridge", "system");
  PRINT_MODULE_USAGE_COMMAND("start");
  PRINT_MODULE_USAGE_DEFAULT_COMMANDS();
  return 0;
}

extern "C" __EXPORT int param_bridge_main(int argc, char *argv[]) {
  return ParamBridge::main(argc, argv);
}
