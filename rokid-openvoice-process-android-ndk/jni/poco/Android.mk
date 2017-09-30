MY_LOCAL_PATH := $(call my-dir)

PocoFoundationDir := poco/Foundation
PocoNetDir := poco/Net
PocoNetSSLDir := poco/NetSSL_OpenSSL
PocoCryptoDir := poco/Crypto
PocoUtilDir := poco/Util
PocoXMLDir := poco/XML
PocoJSONDir := poco/JSON

IGNORE_ERRORS := -Wno-error=non-virtual-dtor

include $(CLEAR_VARS)
Foundation_SRC_FILES := \
	$(PocoFoundationDir)/src/Channel.cpp \
	$(PocoFoundationDir)/src/SplitterChannel.cpp \
	$(PocoFoundationDir)/src/Environment.cpp \
	$(PocoFoundationDir)/src/NullStream.cpp \
	$(PocoFoundationDir)/src/NullChannel.cpp \
	$(PocoFoundationDir)/src/CountingStream.cpp \
	$(PocoFoundationDir)/src/Logger.cpp \
	$(PocoFoundationDir)/src/LoggingFactory.cpp \
	$(PocoFoundationDir)/src/LoggingRegistry.cpp \
	$(PocoFoundationDir)/src/LogStream.cpp \
	$(PocoFoundationDir)/src/DateTime.cpp \
	$(PocoFoundationDir)/src/LocalDateTime.cpp \
	$(PocoFoundationDir)/src/DateTimeFormat.cpp \
	$(PocoFoundationDir)/src/DateTimeFormatter.cpp \
	$(PocoFoundationDir)/src/DateTimeParser.cpp \
	$(PocoFoundationDir)/src/BinaryWriter.cpp \
	$(PocoFoundationDir)/src/BinaryReader.cpp \
	$(PocoFoundationDir)/src/URIStreamFactory.cpp \
	$(PocoFoundationDir)/src/URI.cpp \
	$(PocoFoundationDir)/src/URIStreamOpener.cpp \
	$(PocoFoundationDir)/src/MemoryStream.cpp \
	$(PocoFoundationDir)/src/MemoryPool.cpp \
	$(PocoFoundationDir)/src/Path.cpp \
	$(PocoFoundationDir)/src/File.cpp \
	$(PocoFoundationDir)/src/FileStream.cpp \
	$(PocoFoundationDir)/src/FileStreamFactory.cpp \
	$(PocoFoundationDir)/src/FileChannel.cpp \
	$(PocoFoundationDir)/src/UTF8Encoding.cpp \
	$(PocoFoundationDir)/src/UTF8String.cpp \
	$(PocoFoundationDir)/src/Thread.cpp \
	$(PocoFoundationDir)/src/ThreadLocal.cpp \
	$(PocoFoundationDir)/src/ThreadPool.cpp \
	$(PocoFoundationDir)/src/Random.cpp \
	$(PocoFoundationDir)/src/RandomStream.cpp \
	$(PocoFoundationDir)/src/Notification.cpp \
	$(PocoFoundationDir)/src/NotificationQueue.cpp \
	$(PocoFoundationDir)/src/NotificationCenter.cpp \
	$(PocoFoundationDir)/src/ErrorHandler.cpp \
	$(PocoFoundationDir)/src/Exception.cpp \
	$(PocoFoundationDir)/src/SHA1Engine.cpp \
	$(PocoFoundationDir)/src/Timespan.cpp \
	$(PocoFoundationDir)/src/MD5Engine.cpp \
	$(PocoFoundationDir)/src/Base64Encoder.cpp \
	$(PocoFoundationDir)/src/Base64Decoder.cpp \
	$(PocoFoundationDir)/src/DigestEngine.cpp \
	$(PocoFoundationDir)/src/Message.cpp \
	$(PocoFoundationDir)/src/NumberParser.cpp \
	$(PocoFoundationDir)/src/NumberFormatter.cpp \
	$(PocoFoundationDir)/src/NumericString.cpp \
	$(PocoFoundationDir)/src/StreamCopier.cpp \
	$(PocoFoundationDir)/src/TemporaryFile.cpp \
	$(PocoFoundationDir)/src/Mutex.cpp \
	$(PocoFoundationDir)/src/Timestamp.cpp \
	$(PocoFoundationDir)/src/Process.cpp \
	$(PocoFoundationDir)/src/Runnable.cpp \
	$(PocoFoundationDir)/src/Pipe.cpp \
	$(PocoFoundationDir)/src/PipeImpl.cpp \
	$(PocoFoundationDir)/src/PipeStream.cpp \
	$(PocoFoundationDir)/src/Bugcheck.cpp \
	$(PocoFoundationDir)/src/AtomicCounter.cpp \
	$(PocoFoundationDir)/src/String.cpp \
	$(PocoFoundationDir)/src/StringTokenizer.cpp \
	$(PocoFoundationDir)/src/Format.cpp \
	$(PocoFoundationDir)/src/Formatter.cpp \
	$(PocoFoundationDir)/src/FormattingChannel.cpp \
	$(PocoFoundationDir)/src/Event.cpp \
	$(PocoFoundationDir)/src/EventChannel.cpp \
	$(PocoFoundationDir)/src/EventArgs.cpp \
	$(PocoFoundationDir)/src/Debugger.cpp \
	$(PocoFoundationDir)/src/RefCountedObject.cpp \
	$(PocoFoundationDir)/src/Configurable.cpp \
	$(PocoFoundationDir)/src/TextBufferIterator.cpp \
	$(PocoFoundationDir)/src/TextConverter.cpp \
	$(PocoFoundationDir)/src/TextEncoding.cpp \
	$(PocoFoundationDir)/src/TextIterator.cpp \
	$(PocoFoundationDir)/src/Ascii.cpp \
	$(PocoFoundationDir)/src/ASCIIEncoding.cpp \
	$(PocoFoundationDir)/src/Timezone.cpp \
	$(PocoFoundationDir)/src/Unicode.cpp \
	$(PocoFoundationDir)/src/UnicodeConverter.cpp \
	$(PocoFoundationDir)/src/DirectoryIterator.cpp \
	$(PocoFoundationDir)/src/PatternFormatter.cpp \
	$(PocoFoundationDir)/src/RWLock.cpp \
	$(PocoFoundationDir)/src/UTF16Encoding.cpp \
	$(PocoFoundationDir)/src/UTF32Encoding.cpp \
	$(PocoFoundationDir)/src/SyslogChannel.cpp \
	$(PocoFoundationDir)/src/ConsoleChannel.cpp \
	$(PocoFoundationDir)/src/RotateStrategy.cpp \
	$(PocoFoundationDir)/src/ArchiveStrategy.cpp \
	$(PocoFoundationDir)/src/LogFile.cpp \
	$(PocoFoundationDir)/src/AsyncChannel.cpp \
	$(PocoFoundationDir)/src/PurgeStrategy.cpp \
	$(PocoFoundationDir)/src/DeflatingStream.cpp \
	$(PocoFoundationDir)/src/RegularExpression.cpp \
	$(PocoFoundationDir)/src/deflate.c \
	$(PocoFoundationDir)/src/adler32.c \
	$(PocoFoundationDir)/src/compress.c \
	$(PocoFoundationDir)/src/crc32.c \
	$(PocoFoundationDir)/src/infback.c \
	$(PocoFoundationDir)/src/inffast.c \
	$(PocoFoundationDir)/src/inflate.c \
	$(PocoFoundationDir)/src/inftrees.c \
	$(PocoFoundationDir)/src/trees.c \
	$(PocoFoundationDir)/src/zutil.c \
	$(PocoFoundationDir)/src/pcre_config.c \
	$(PocoFoundationDir)/src/pcre_chartables.c \
	$(PocoFoundationDir)/src/pcre_compile.c \
	$(PocoFoundationDir)/src/pcre_globals.c \
	$(PocoFoundationDir)/src/pcre_maketables.c \
	$(PocoFoundationDir)/src/pcre_get.c \
	$(PocoFoundationDir)/src/pcre_study.c \
	$(PocoFoundationDir)/src/pcre_byte_order.c \
	$(PocoFoundationDir)/src/pcre_valid_utf8.c \
	$(PocoFoundationDir)/src/pcre_dfa_exec.c \
	$(PocoFoundationDir)/src/pcre_jit_compile.c \
	$(PocoFoundationDir)/src/pcre_exec.c \
	$(PocoFoundationDir)/src/pcre_ord2utf8.c \
	$(PocoFoundationDir)/src/pcre_newline.c \
	$(PocoFoundationDir)/src/pcre_fullinfo.c \
	$(PocoFoundationDir)/src/pcre_xclass.c \
	$(PocoFoundationDir)/src/pcre_refcount.c \
	$(PocoFoundationDir)/src/pcre_string_utils.c \
	$(PocoFoundationDir)/src/pcre_version.c \
	$(PocoFoundationDir)/src/pcre_ucd.c \
	$(PocoFoundationDir)/src/pcre_tables.c \
	$(PocoFoundationDir)/src/Latin1Encoding.cpp \
	$(PocoFoundationDir)/src/Latin2Encoding.cpp \
	$(PocoFoundationDir)/src/Latin9Encoding.cpp \
	$(PocoFoundationDir)/src/Windows1250Encoding.cpp \
	$(PocoFoundationDir)/src/Windows1251Encoding.cpp \
	$(PocoFoundationDir)/src/Windows1252Encoding.cpp \
	$(PocoFoundationDir)/src/ActiveDispatcher.cpp \
	$(PocoFoundationDir)/src/TimedNotificationQueue.cpp \
	$(PocoFoundationDir)/src/HexBinaryEncoder.cpp \
	$(PocoFoundationDir)/src/HexBinaryDecoder.cpp \
	$(PocoFoundationDir)/src/Clock.cpp \
	$(PocoFoundationDir)/src/Var.cpp \
	$(PocoFoundationDir)/src/VarHolder.cpp \
	$(PocoFoundationDir)/src/StreamConverter.cpp \
	$(PocoFoundationDir)/src/LineEndingConverter.cpp \
	$(PocoFoundationDir)/src/SignalHandler.cpp \

Net_SRC_FILES := \
	$(PocoNetDir)/src/AbstractHTTPRequestHandler.cpp \
	$(PocoNetDir)/src/DatagramSocket.cpp \
	$(PocoNetDir)/src/DatagramSocketImpl.cpp \
	$(PocoNetDir)/src/DialogSocket.cpp \
	$(PocoNetDir)/src/DNS.cpp \
	$(PocoNetDir)/src/FilePartSource.cpp \
	$(PocoNetDir)/src/FTPClientSession.cpp \
	$(PocoNetDir)/src/FTPStreamFactory.cpp \
	$(PocoNetDir)/src/HostEntry.cpp \
	$(PocoNetDir)/src/HTMLForm.cpp \
	$(PocoNetDir)/src/HTTPAuthenticationParams.cpp \
	$(PocoNetDir)/src/HTTPBasicCredentials.cpp \
	$(PocoNetDir)/src/HTTPBufferAllocator.cpp \
	$(PocoNetDir)/src/HTTPChunkedStream.cpp \
	$(PocoNetDir)/src/HTTPClientSession.cpp \
	$(PocoNetDir)/src/HTTPCookie.cpp \
	$(PocoNetDir)/src/HTTPCredentials.cpp \
	$(PocoNetDir)/src/HTTPDigestCredentials.cpp \
	$(PocoNetDir)/src/HTTPFixedLengthStream.cpp \
	$(PocoNetDir)/src/HTTPHeaderStream.cpp \
	$(PocoNetDir)/src/HTTPIOStream.cpp \
	$(PocoNetDir)/src/HTTPMessage.cpp \
	$(PocoNetDir)/src/HTTPRequest.cpp \
	$(PocoNetDir)/src/HTTPRequestHandler.cpp \
	$(PocoNetDir)/src/HTTPRequestHandlerFactory.cpp \
	$(PocoNetDir)/src/HTTPResponse.cpp \
	$(PocoNetDir)/src/HTTPServerConnection.cpp \
	$(PocoNetDir)/src/HTTPServerConnectionFactory.cpp \
	$(PocoNetDir)/src/HTTPServer.cpp \
	$(PocoNetDir)/src/HTTPServerParams.cpp \
	$(PocoNetDir)/src/HTTPServerRequest.cpp \
	$(PocoNetDir)/src/HTTPServerRequestImpl.cpp \
	$(PocoNetDir)/src/HTTPServerResponse.cpp \
	$(PocoNetDir)/src/HTTPServerResponseImpl.cpp \
	$(PocoNetDir)/src/HTTPServerSession.cpp \
	$(PocoNetDir)/src/HTTPSession.cpp \
	$(PocoNetDir)/src/HTTPSessionFactory.cpp \
	$(PocoNetDir)/src/HTTPSessionInstantiator.cpp \
	$(PocoNetDir)/src/HTTPStream.cpp \
	$(PocoNetDir)/src/HTTPStreamFactory.cpp \
	$(PocoNetDir)/src/ICMPClient.cpp \
	$(PocoNetDir)/src/ICMPEventArgs.cpp \
	$(PocoNetDir)/src/ICMPPacket.cpp \
	$(PocoNetDir)/src/ICMPPacketImpl.cpp \
	$(PocoNetDir)/src/ICMPSocket.cpp \
	$(PocoNetDir)/src/ICMPSocketImpl.cpp \
	$(PocoNetDir)/src/ICMPv4PacketImpl.cpp \
	$(PocoNetDir)/src/IPAddress.cpp \
	$(PocoNetDir)/src/IPAddressImpl.cpp \
	$(PocoNetDir)/src/MailMessage.cpp \
	$(PocoNetDir)/src/MailRecipient.cpp \
	$(PocoNetDir)/src/MailStream.cpp \
	$(PocoNetDir)/src/MediaType.cpp \
	$(PocoNetDir)/src/MessageHeader.cpp \
	$(PocoNetDir)/src/MulticastSocket.cpp \
	$(PocoNetDir)/src/MultipartReader.cpp \
	$(PocoNetDir)/src/MultipartWriter.cpp \
	$(PocoNetDir)/src/NameValueCollection.cpp \
	$(PocoNetDir)/src/Net.cpp \
	$(PocoNetDir)/src/NetException.cpp \
	$(PocoNetDir)/src/NetworkInterface.cpp \
	$(PocoNetDir)/src/NTPClient.cpp \
	$(PocoNetDir)/src/NTPEventArgs.cpp \
	$(PocoNetDir)/src/NTPPacket.cpp \
	$(PocoNetDir)/src/NullPartHandler.cpp \
	$(PocoNetDir)/src/OAuth10Credentials.cpp \
	$(PocoNetDir)/src/OAuth20Credentials.cpp \
	$(PocoNetDir)/src/PartHandler.cpp \
	$(PocoNetDir)/src/PartSource.cpp \
	$(PocoNetDir)/src/PartStore.cpp \
	$(PocoNetDir)/src/POP3ClientSession.cpp \
	$(PocoNetDir)/src/QuotedPrintableDecoder.cpp \
	$(PocoNetDir)/src/QuotedPrintableEncoder.cpp \
	$(PocoNetDir)/src/RawSocket.cpp \
	$(PocoNetDir)/src/RawSocketImpl.cpp \
	$(PocoNetDir)/src/RemoteSyslogChannel.cpp \
	$(PocoNetDir)/src/RemoteSyslogListener.cpp \
	$(PocoNetDir)/src/ServerSocket.cpp \
	$(PocoNetDir)/src/ServerSocketImpl.cpp \
	$(PocoNetDir)/src/SMTPChannel.cpp \
	$(PocoNetDir)/src/SMTPClientSession.cpp \
	$(PocoNetDir)/src/SocketAddress.cpp \
	$(PocoNetDir)/src/SocketAddressImpl.cpp \
	$(PocoNetDir)/src/Socket.cpp \
	$(PocoNetDir)/src/SocketImpl.cpp \
	$(PocoNetDir)/src/SocketNotification.cpp \
	$(PocoNetDir)/src/SocketNotifier.cpp \
	$(PocoNetDir)/src/SocketReactor.cpp \
	$(PocoNetDir)/src/SocketStream.cpp \
	$(PocoNetDir)/src/StreamSocket.cpp \
	$(PocoNetDir)/src/StreamSocketImpl.cpp \
	$(PocoNetDir)/src/StringPartSource.cpp \
	$(PocoNetDir)/src/TCPServerConnection.cpp \
	$(PocoNetDir)/src/TCPServerConnectionFactory.cpp \
	$(PocoNetDir)/src/TCPServer.cpp \
	$(PocoNetDir)/src/TCPServerDispatcher.cpp \
	$(PocoNetDir)/src/TCPServerParams.cpp \
	$(PocoNetDir)/src/WebSocket.cpp \
	$(PocoNetDir)/src/WebSocketImpl.cpp

NetSSL_SRC_FILES := \
	$(PocoNetSSLDir)/src/AcceptCertificateHandler.cpp \
	$(PocoNetSSLDir)/src/CertificateHandlerFactory.cpp \
	$(PocoNetSSLDir)/src/CertificateHandlerFactoryMgr.cpp \
	$(PocoNetSSLDir)/src/ConsoleCertificateHandler.cpp \
	$(PocoNetSSLDir)/src/Context.cpp \
	$(PocoNetSSLDir)/src/HTTPSClientSession.cpp \
	$(PocoNetSSLDir)/src/HTTPSSessionInstantiator.cpp \
	$(PocoNetSSLDir)/src/HTTPSStreamFactory.cpp \
	$(PocoNetSSLDir)/src/InvalidCertificateHandler.cpp \
	$(PocoNetSSLDir)/src/KeyConsoleHandler.cpp \
	$(PocoNetSSLDir)/src/KeyFileHandler.cpp \
	$(PocoNetSSLDir)/src/PrivateKeyFactory.cpp \
	$(PocoNetSSLDir)/src/PrivateKeyFactoryMgr.cpp \
	$(PocoNetSSLDir)/src/PrivateKeyPassphraseHandler.cpp \
	$(PocoNetSSLDir)/src/RejectCertificateHandler.cpp \
	$(PocoNetSSLDir)/src/SecureServerSocket.cpp \
	$(PocoNetSSLDir)/src/SecureServerSocketImpl.cpp \
	$(PocoNetSSLDir)/src/SecureSMTPClientSession.cpp \
	$(PocoNetSSLDir)/src/SecureSocketImpl.cpp \
	$(PocoNetSSLDir)/src/SecureStreamSocket.cpp \
	$(PocoNetSSLDir)/src/SecureStreamSocketImpl.cpp \
	$(PocoNetSSLDir)/src/Session.cpp \
	$(PocoNetSSLDir)/src/SSLException.cpp \
	$(PocoNetSSLDir)/src/SSLManager.cpp \
	$(PocoNetSSLDir)/src/Utility.cpp \
	$(PocoNetSSLDir)/src/VerificationErrorArgs.cpp \
	$(PocoNetSSLDir)/src/X509Certificate.cpp

Crypto_SRC_FILES := \
	$(PocoCryptoDir)/src/Cipher.cpp \
	$(PocoCryptoDir)/src/CipherFactory.cpp \
	$(PocoCryptoDir)/src/CipherImpl.cpp \
	$(PocoCryptoDir)/src/CipherKey.cpp \
	$(PocoCryptoDir)/src/CipherKeyImpl.cpp \
	$(PocoCryptoDir)/src/CryptoStream.cpp \
	$(PocoCryptoDir)/src/CryptoTransform.cpp \
	$(PocoCryptoDir)/src/DigestEngine.cpp \
	$(PocoCryptoDir)/src/OpenSSLInitializer.cpp \
	$(PocoCryptoDir)/src/RSACipherImpl.cpp \
	$(PocoCryptoDir)/src/RSADigestEngine.cpp \
	$(PocoCryptoDir)/src/RSAKey.cpp \
	$(PocoCryptoDir)/src/RSAKeyImpl.cpp \
	$(PocoCryptoDir)/src/X509Certificate.cpp

Util_SRC_FILES := \
	$(PocoUtilDir)/src/AbstractConfiguration.cpp \
	$(PocoUtilDir)/src/Application.cpp \
	$(PocoUtilDir)/src/ConfigurationMapper.cpp \
	$(PocoUtilDir)/src/ConfigurationView.cpp \
	$(PocoUtilDir)/src/FilesystemConfiguration.cpp \
	$(PocoUtilDir)/src/HelpFormatter.cpp \
	$(PocoUtilDir)/src/IniFileConfiguration.cpp \
	$(PocoUtilDir)/src/IntValidator.cpp \
	$(PocoUtilDir)/src/JSONConfiguration.cpp \
	$(PocoUtilDir)/src/LayeredConfiguration.cpp \
	$(PocoUtilDir)/src/LoggingConfigurator.cpp \
	$(PocoUtilDir)/src/LoggingSubsystem.cpp \
	$(PocoUtilDir)/src/MapConfiguration.cpp \
	$(PocoUtilDir)/src/OptionCallback.cpp \
	$(PocoUtilDir)/src/Option.cpp \
	$(PocoUtilDir)/src/OptionException.cpp \
	$(PocoUtilDir)/src/OptionProcessor.cpp \
	$(PocoUtilDir)/src/OptionSet.cpp \
	$(PocoUtilDir)/src/PropertyFileConfiguration.cpp \
	$(PocoUtilDir)/src/RegExpValidator.cpp \
	$(PocoUtilDir)/src/ServerApplication.cpp \
	$(PocoUtilDir)/src/Subsystem.cpp \
	$(PocoUtilDir)/src/SystemConfiguration.cpp \
	$(PocoUtilDir)/src/Timer.cpp \
	$(PocoUtilDir)/src/TimerTask.cpp \
	$(PocoUtilDir)/src/Validator.cpp \
	$(PocoUtilDir)/src/XMLConfiguration.cpp

XML_SRC_FILES := \
	$(PocoXMLDir)/src/AbstractContainerNode.cpp \
	$(PocoXMLDir)/src/AbstractNode.cpp \
	$(PocoXMLDir)/src/Attr.cpp \
	$(PocoXMLDir)/src/Attributes.cpp \
	$(PocoXMLDir)/src/AttributesImpl.cpp \
	$(PocoXMLDir)/src/AttrMap.cpp \
	$(PocoXMLDir)/src/CDATASection.cpp \
	$(PocoXMLDir)/src/CharacterData.cpp \
	$(PocoXMLDir)/src/ChildNodesList.cpp \
	$(PocoXMLDir)/src/Comment.cpp \
	$(PocoXMLDir)/src/ContentHandler.cpp \
	$(PocoXMLDir)/src/DeclHandler.cpp \
	$(PocoXMLDir)/src/DefaultHandler.cpp \
	$(PocoXMLDir)/src/Document.cpp \
	$(PocoXMLDir)/src/DocumentEvent.cpp \
	$(PocoXMLDir)/src/DocumentFragment.cpp \
	$(PocoXMLDir)/src/DocumentType.cpp \
	$(PocoXMLDir)/src/DOMBuilder.cpp \
	$(PocoXMLDir)/src/DOMException.cpp \
	$(PocoXMLDir)/src/DOMImplementation.cpp \
	$(PocoXMLDir)/src/DOMObject.cpp \
	$(PocoXMLDir)/src/DOMParser.cpp \
	$(PocoXMLDir)/src/DOMSerializer.cpp \
	$(PocoXMLDir)/src/DOMWriter.cpp \
	$(PocoXMLDir)/src/DTDHandler.cpp \
	$(PocoXMLDir)/src/DTDMap.cpp \
	$(PocoXMLDir)/src/Element.cpp \
	$(PocoXMLDir)/src/ElementsByTagNameList.cpp \
	$(PocoXMLDir)/src/Entity.cpp \
	$(PocoXMLDir)/src/EntityReference.cpp \
	$(PocoXMLDir)/src/EntityResolver.cpp \
	$(PocoXMLDir)/src/EntityResolverImpl.cpp \
	$(PocoXMLDir)/src/ErrorHandler.cpp \
	$(PocoXMLDir)/src/Event.cpp \
	$(PocoXMLDir)/src/EventDispatcher.cpp \
	$(PocoXMLDir)/src/EventException.cpp \
	$(PocoXMLDir)/src/EventListener.cpp \
	$(PocoXMLDir)/src/EventTarget.cpp \
	$(PocoXMLDir)/src/InputSource.cpp \
	$(PocoXMLDir)/src/LexicalHandler.cpp \
	$(PocoXMLDir)/src/Locator.cpp \
	$(PocoXMLDir)/src/LocatorImpl.cpp \
	$(PocoXMLDir)/src/MutationEvent.cpp \
	$(PocoXMLDir)/src/Name.cpp \
	$(PocoXMLDir)/src/NamedNodeMap.cpp \
	$(PocoXMLDir)/src/NamePool.cpp \
	$(PocoXMLDir)/src/NamespaceStrategy.cpp \
	$(PocoXMLDir)/src/NamespaceSupport.cpp \
	$(PocoXMLDir)/src/NodeAppender.cpp \
	$(PocoXMLDir)/src/Node.cpp \
	$(PocoXMLDir)/src/NodeFilter.cpp \
	$(PocoXMLDir)/src/NodeIterator.cpp \
	$(PocoXMLDir)/src/NodeList.cpp \
	$(PocoXMLDir)/src/Notation.cpp \
	$(PocoXMLDir)/src/ParserEngine.cpp \
	$(PocoXMLDir)/src/ProcessingInstruction.cpp \
	$(PocoXMLDir)/src/SAXException.cpp \
	$(PocoXMLDir)/src/SAXParser.cpp \
	$(PocoXMLDir)/src/Text.cpp \
	$(PocoXMLDir)/src/TreeWalker.cpp \
	$(PocoXMLDir)/src/WhitespaceFilter.cpp \
	$(PocoXMLDir)/src/XMLException.cpp \
	$(PocoXMLDir)/src/XMLFilter.cpp \
	$(PocoXMLDir)/src/XMLFilterImpl.cpp \
	$(PocoXMLDir)/src/XMLReader.cpp \
	$(PocoXMLDir)/src/XMLString.cpp \
	$(PocoXMLDir)/src/XMLWriter.cpp \
	$(PocoXMLDir)/src/xmlparse.cpp \
	$(PocoXMLDir)/src/xmltok.c \
	$(PocoXMLDir)/src/xmlrole.c \

JSON_SRC_FILES := \
	$(PocoJSONDir)/src/Array.cpp \
	$(PocoJSONDir)/src/Handler.cpp \
	$(PocoJSONDir)/src/JSONException.cpp \
	$(PocoJSONDir)/src/Object.cpp \
	$(PocoJSONDir)/src/ParseHandler.cpp \
	$(PocoJSONDir)/src/Parser.cpp \
	$(PocoJSONDir)/src/PrintHandler.cpp \
	$(PocoJSONDir)/src/Query.cpp \
	$(PocoJSONDir)/src/Stringifier.cpp \
	$(PocoJSONDir)/src/TemplateCache.cpp \
	$(PocoJSONDir)/src/Template.cpp

LOCAL_SRC_FILES := $(Foundation_SRC_FILES) $(Net_SRC_FILES) $(NetSSL_SRC_FILES) \
	$(Crypto_SRC_FILES) $(Util_SRC_FILES) $(XML_SRC_FILES) $(JSON_SRC_FILES)

LOCAL_MODULE := libpoco
LOCAL_CFLAGS := -DPOCO_ANDROID -DPOCO_NO_FPENVIRONMENT -DPOCO_NO_WSTRING \
	-DPOCO_NO_SHAREDMEMORY -DHAVE_EXPAT_CONFIG_H
LOCAL_CPPFLAGS := -frtti -fexceptions -std=c++11 $(IGNORE_ERRORS) 

ifeq ($(ANDROID_VERSION), 21)
LOCAL_CPPFLAGS += -DUSE_BORING_SSL
endif

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/$(PocoFoundationDir)/include \
	$(LOCAL_PATH)/$(PocoNetDir)/include \
	$(LOCAL_PATH)/$(PocoNetSSLDir)/include \
	$(LOCAL_PATH)/$(PocoCryptoDir)/include \
	$(LOCAL_PATH)/$(PocoUtilDir)/include \
	$(LOCAL_PATH)/$(PocoXMLDir)/include \
	$(LOCAL_PATH)/$(PocoJSONDir)/include \
	$(LOCAL_PATH)/include/$(ANDROID_VERSION)

LOCAL_SHARED_LIBRARIES := libssl libcrypto

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
include $(BUILD_SHARED_LIBRARY)
