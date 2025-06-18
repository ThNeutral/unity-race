import asyncio

TARGET_IP = '127.0.0.1'
TARGET_PORT = 8080
TOTAL_REQUESTS = 1000

class UDPClientProtocol(asyncio.DatagramProtocol):
    def __init__(self, message, on_con_lost):
        self.message = message.encode()
        self.on_con_lost = on_con_lost

    def connection_made(self, transport):
        transport.sendto(self.message)
        transport.close()

    def connection_lost(self, exc):
        self.on_con_lost.set_result(True)

async def send_udp_message(message, loop):
    on_con_lost = loop.create_future()
    transport, protocol = await loop.create_datagram_endpoint(
        lambda: UDPClientProtocol(message, on_con_lost),
        remote_addr=(TARGET_IP, TARGET_PORT)
    )
    try:
        await on_con_lost
    finally:
        transport.close()

async def main():
    loop = asyncio.get_event_loop()
    tasks = [send_udp_message(f"Message {i}", loop) for i in range(1, TOTAL_REQUESTS + 1)]
    await asyncio.gather(*tasks)

if __name__ == '__main__':
    asyncio.run(main())