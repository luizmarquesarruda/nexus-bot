"""Initial CLI for checking the Orange Pi ↔ ESP32 serial link."""
from __future__ import annotations

import argparse
import logging
import time

from .uart_link import UartLink


def main() -> None:
    parser = argparse.ArgumentParser(description="NEXUS BOT UART link smoke test")
    parser.add_argument("--port", required=True, help="Serial device, e.g. /dev/ttyS1 or /dev/ttyUSB0")
    parser.add_argument("--baud", type=int, default=115200)
    args = parser.parse_args()

    logging.basicConfig(level=logging.INFO, format="%(levelname)s %(message)s")
    logging.info("Abrindo UART; nenhum movimento será comandado neste teste.")
    with UartLink(args.port, args.baud) as link:
        link.stop()
        deadline = time.monotonic() + 2.0
        while time.monotonic() < deadline:
            response = link.read_response()
            if response is not None:
                logging.info("Resposta ESP32: %s", response)
                return
        logging.warning("Nenhuma resposta recebida. Verifique alimentação lógica, GND, RX/TX e baud rate.")


if __name__ == "__main__":
    main()
