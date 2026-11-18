"""Small serial transport; motor safety remains enforced by ESP32 firmware."""
from __future__ import annotations

from typing import Any

import serial

from .protocol import MAX_FRAME_BYTES, drive_frame, parse_response, stop_frame


class UartLink:
    def __init__(self, port: str, baudrate: int = 115200, timeout: float = 0.15) -> None:
        self._serial = serial.Serial(port=port, baudrate=baudrate, timeout=timeout, write_timeout=timeout)
        self._seq = 0

    def _next_seq(self) -> int:
        self._seq = (self._seq + 1) & 0x7FFFFFFF
        return self._seq

    def _send(self, frame: bytes) -> None:
        self._serial.write(frame)
        self._serial.flush()

    def drive(self, left: float, right: float, ttl_ms: int = 200) -> None:
        self._send(drive_frame(self._next_seq(), left, right, ttl_ms))

    def stop(self) -> None:
        self._send(stop_frame(self._next_seq()))

    def read_response(self) -> dict[str, Any] | None:
        raw = self._serial.read_until(expected=b"\n", size=MAX_FRAME_BYTES + 1)
        if not raw:
            return None
        if len(raw) > MAX_FRAME_BYTES or not raw.endswith(b"\n"):
            self._serial.reset_input_buffer()
            raise ValueError("UART response is incomplete or exceeds frame limit")
        return parse_response(raw.strip())

    def close(self) -> None:
        try:
            self.stop()
        finally:
            self._serial.close()

    def __enter__(self) -> "UartLink":
        return self

    def __exit__(self, exc_type: object, exc: object, traceback: object) -> None:
        self.close()
