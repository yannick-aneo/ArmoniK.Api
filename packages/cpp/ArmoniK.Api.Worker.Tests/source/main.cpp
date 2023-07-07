#include <iostream>
#include <memory>
#include <string>

#include <grpc++/grpc++.h>

#include "grpcpp/support/sync_stream.h"
#include "objects.pb.h"

#include "utils/RootConfiguration.h"
#include "utils/WorkerServer.h"
#include "worker_common.grpc.pb.h"
#include "worker_service.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using armonik::api::common::utils::IConfiguration;
using armonik::api::grpc::v1::TaskOptions;

using namespace armonik::api::grpc::v1::worker;
using namespace armonik::api::worker;
using namespace armonik::api::common::utils;

/**
 * @brief Implements the Worker service.
 */
class WorkerServiceImpl final : public Worker::Service {
private:
  armonik::api::common::serilog::serilog logger;

public:
  /**
   * @brief Constructs a WorkerServiceImpl object.
   */
  WorkerServiceImpl() : logger(armonik::api::common::serilog::logging_format::SEQ) {
    logger.info("Build Service WorkerServiceImpl");
    logger.add_property("class", "WorkerServiceImpl");
    logger.add_property("Worker", "ArmoniK.Api.Cpp");
  }

  /**
   * @brief Implements the Process method of the Worker service.
   *
   * @param context The ServerContext object.
   * @param reader The ServerReader object.
   * @param response The ProcessReply object.
   *
   * @return The status of the method.
   */
  Status Process(::grpc::ServerContext *context,
                 ::grpc::ServerReader<::armonik::api::grpc::v1::worker::ProcessRequest> *reader,
                 ::armonik::api::grpc::v1::worker::ProcessReply *response) override {
    // Implementation of the Process method
    logger.info("Receive new request From C++ Worker");
    auto output = armonik::api::grpc::v1::Output();
    *output.mutable_ok() = armonik::api::grpc::v1::Empty();
    ProcessRequest req;
    reader->Read(&req);
    *response->mutable_output() = output;

    logger.info("Finish call C++");

    return grpc::Status::OK;
  }

  /**
   * @brief Implements the HealthCheck method of the Worker service.
   *
   * @param context The ServerContext object.
   * @param request The Empty object.
   * @param response The HealthCheckReply object.
   *
   * @return The status of the method.
   */
  Status HealthCheck(::grpc::ServerContext *context, const ::armonik::api::grpc::v1::Empty *request,
                     ::armonik::api::grpc::v1::worker::HealthCheckReply *response) override {
    // Implementation of the HealthCheck method
    logger.info("HealthCheck request OK");

    response->set_status(HealthCheckReply_ServingStatus_SERVING);

    return Status::OK;
  }
};

int main(int argc, char **argv) {
  std::cout << "Starting C++ worker..." << std::endl;

  std::shared_ptr<IConfiguration> config = std::make_shared<RootConfiguration>();

  config->set("ComputePlane__WorkerChannel__Address", "/cache/armonik_worker.sock");
  config->set("ComputePlane__AgentChannel__Address", "/cache/armonik_agent.sock");

  config->get_compute_plane();
  WorkerServer::create<WorkerServiceImpl, bool>(config)->run();

  std::cout << "Stooping Server..." << std::endl;
  return 0;
}
