/*
 * ======================================================================
 * Copyright (c) Microsoft Open Technologies, Inc.  All rights reserved.
 * Licensed under the MIT License.
 * See LICENSE.md in the project root for license information.
 * ======================================================================
 */

#ifndef _RMS_LIB_QDATASTREAM_H_
#define _RMS_LIB_QDATASTREAM_H_

#include <mutex>
#include "../ModernAPIExport.h"
// #include <QSharedPointer>
//#include <QDataStream>
#include <CryptoAPI.h>

template<class T> struct StreamMethods {
  static uint64_t pos(std::shared_ptr<T> stream) {
    return stream->device()->pos();
  }
  static void seek(std::shared_ptr<T> stream, int64_t pos) {
    stream->device()->seek(pos);
  }
  static uint64_t size(std::shared_ptr<T> stream) {
    return stream->device()->size();
  }
  static bool canRead(std::shared_ptr<T> stream) {
    // QIODeviceBase::ReadOnly = 0x01
    return stream->device()->openMode() & 0x01;
  }
  static bool canWrite(std::shared_ptr<T> stream) {
    // QIODeviceBase::ReadOnly = 0x02
    return stream->device()->openMode() & 0x02;
  }
  static std::shared_ptr<T> clone(std::shared_ptr<T> stream) {
    return std::make_shared<T>(stream->device());
  }
  static int64_t read(std::shared_ptr<T> stream, uint8_t* buff, int64_t size) {
    return stream->readRawData((char*)buff, (int)size);
  }
  static int64_t write(std::shared_ptr<T> stream, const uint8_t* buff, int64_t size) {
    return stream->writeRawData((const char*)buff, (int)size);
  }
};

template<class T>
class DLL_PUBLIC_RMS StreamImpl :
  public rmscrypto::api::IStream,
  public std::enable_shared_from_this<StreamImpl<T>>{
public:
  typedef StreamMethods<T> Methods;
  static rmscrypto::api::SharedStream Create(std::shared_ptr<T> stream) {
    auto self = new StreamImpl<T>(stream);
    return std::static_pointer_cast<rmscrypto::api::IStream>(std::shared_ptr<StreamImpl<T>>(self));
  }
  virtual ~StreamImpl() {}


  virtual std::shared_future<int64_t>ReadAsync(uint8_t    *pbBuffer,
                                               int64_t     cbBuffer,
                                               int64_t     cbOffset,
                                               std::launch launchType)
  override
  {
    auto selfPtr = this->shared_from_this();
    return std::async(launchType, [](
      std::shared_ptr<StreamImpl<T>> self,
      uint8_t* buffer,
      int64_t size,
      int64_t offset) -> int64_t {
      
      std::unique_lock<std::mutex> lock(self->locker_);
      StreamMethods<T>::seek(self->stream_, offset);
      return StreamMethods<T>::read(self->stream_, buffer, size);
      
    }, selfPtr, pbBuffer, cbBuffer, cbOffset);
  }

  virtual std::shared_future<int64_t>WriteAsync(const uint8_t *cpbBuffer,
                                                int64_t        cbBuffer,
                                                int64_t        cbOffset,
                                                std::launch    launchType)
  override
  {
    auto selfPtr = this->shared_from_this();
    return std::async(launchType, [](
      std::shared_ptr<StreamImpl<T>> self,
      const uint8_t* buffer,
      int64_t size,
      int64_t offset) -> int64_t {
      
      std::unique_lock<std::mutex> lock(self->locker_);
      StreamMethods<T>::seek(self->stream_, offset);
      //Logger::Hidden("Write %I64d bytes at offset = %I64d from buffer = %x", size, offset, buffer);
      return StreamMethods<T>::write(self->stream_, buffer, size);
      
    }, selfPtr, cpbBuffer, cbBuffer, cbOffset);
  }

  virtual std::future<bool>            FlushAsync(std::launch /*launchType*/)
  override
  {
    std::promise<bool> val;
    auto res = val.get_future();
    val.set_value(true);
    return res;
  }

  virtual int64_t                      Read(uint8_t *pbBuffer,
                                            int64_t  cbBuffer) override
  {
    std::unique_lock<std::mutex> lock(locker_);
    return Methods::read(stream_, pbBuffer, cbBuffer);
  }

  virtual int64_t                      Write(const uint8_t *cpbBuffer,
                                             int64_t        cbBuffer) override
  {
    std::unique_lock<std::mutex> lock(locker_);
    //Logger::Hidden("Write %I64d bytes at offset = %I64d from buffer = %x", cbBuffer, Methods::pos(stream_), buffer);
    return Methods::write(stream_, cpbBuffer, cbBuffer);
  }

  virtual bool                         Flush() override
  {
    return true;
  }

  virtual rmscrypto::api::SharedStream Clone() override
  {
    std::unique_lock<std::mutex> lock(locker_);
    return Create(Methods::clone(stream_));
  }

  virtual void                         Seek(uint64_t u64Position) override
  {
    std::unique_lock<std::mutex> lock(locker_);
    Methods::seek(stream_, u64Position);
  }

  virtual bool                         CanRead()  const           override
  {
    return Methods::canRead(stream_);
  }

  virtual bool                         CanWrite() const           override
  {
    return Methods::canWrite(stream_);
  }

  virtual uint64_t                     Position()                 override
  {
    std::unique_lock<std::mutex> lock(locker_);
    return Methods::pos(stream_);
  }

  virtual uint64_t                     Size()                     override
  {
    std::unique_lock<std::mutex> lock(locker_);
    return Methods::size(stream_);
  }

  virtual void                         Size(uint64_t /* u64Value */)    override
  {}

private:

  StreamImpl(std::shared_ptr<T> stream): stream_(stream) {}
  StreamImpl() = delete;

  std::shared_ptr<T> stream_;
  std::mutex locker_; // QDataStream is not thread safe!!!
};

class DLL_PUBLIC_RMS QTStreamImpl {
public:
  template<class T>
  static rmscrypto::api::SharedStream Create(std::shared_ptr<T> stream) {
    return StreamImpl<T>::Create(stream);
  }
};

#endif // ifndef _RMS_LIB_QDATASTREAM_H_
