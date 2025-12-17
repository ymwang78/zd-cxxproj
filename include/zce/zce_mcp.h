#pragma once
#include <zce/http_stream.h>
#include <nlohmann/json.hpp>
#include <string>
#include <map>
#include <functional>

namespace zce {

// 前置声明
class McpHost;

class McpStream : public HttpStream {
  public:
    explicit McpStream(McpHost* host);
    ~McpStream() override;

    void on_open(bool passive, const zce_sockaddr_t& remote) override;
    void on_close() override;

    void on_http_request(const SmartPtr<ZCE_HTTP_REQUEST>& req, const RefBlock& dblock) override;

    int write(RefBlock& dblock, ERV_ISTREAM_WRITEOPT opt = ERV_ISTREAM_DEFAULT) override;

    void close() override;

  public:
    // 用于 host 写回 JSON-RPC response
    int sendJsonRpc(const nlohmann::json& msg);
    int sendJsonRpcResult(const nlohmann::json& id, const nlohmann::json& result);
    int sendJsonRpcError(const nlohmann::json& id, int code, const std::string& message,
                         const nlohmann::json& data = nullptr);

    bool isInitialized() const { return initialized_; }
    void markInitialized() { initialized_ = true; }

    const std::string& negotiatedProtocolVersion() const { return protocol_version_; }
    void setNegotiatedProtocolVersion(std::string v) { protocol_version_ = std::move(v); }

  private:
    int handleIncomingJson(const RefBlock& dblock);

  private:
    McpHost* host_ = nullptr;
    bool initialized_ = false;
    std::string protocol_version_ = "2025-11-25";
};

class McpHost : public Object {
    struct Impl;
    struct Impl* pimpl_ = nullptr;
  public:
    enum class Protocol { HTTP, HTTPS, WEBSOCKET, WEBSOCKETS };

    McpHost();
    virtual ~McpHost();

    int bind(const zce::SmartPtr<zce::Reactor>& reactor, const std::string& address, int port);

    // ===== MCP JSON-RPC 分发入口（由 McpStream 调用）=====
    int handleJsonRpc(const SmartPtr<McpStream>& stream, const nlohmann::json& msg);

  protected:
    // ===== 必选：Lifecycle =====
    virtual nlohmann::json serverInfo() const;
    virtual nlohmann::json serverCapabilities() const;

    // ===== 标准方法处理 =====
    virtual int onInitialize(const SmartPtr<McpStream>& stream, const nlohmann::json& id,
                             const nlohmann::json& params);

    virtual int onToolsList(const SmartPtr<McpStream>& stream, const nlohmann::json& id,
                            const nlohmann::json& params) = 0;

    virtual int onToolsCall(const SmartPtr<McpStream>& stream, const nlohmann::json& id,
                            const nlohmann::json& params) = 0;

    virtual int onResourcesList(const SmartPtr<McpStream>& stream, const nlohmann::json& id,
                                const nlohmann::json& params) = 0;

    virtual int onResourcesRead(const SmartPtr<McpStream>& stream, const nlohmann::json& id,
                                const nlohmann::json& params) = 0;

    // 可选：ping / logging / prompts / completions 等后续再加

  private:
    static bool isRequest(const nlohmann::json& msg);
    static std::string getMethodOrEmpty(const nlohmann::json& msg);
    static nlohmann::json getParamsOrEmptyObj(const nlohmann::json& msg);
};

}  // namespace zce