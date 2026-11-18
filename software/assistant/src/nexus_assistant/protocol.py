"""Versioned newline-delimited JSON protocol for the ESP32 link."""
from __future__ import annotations

import json
import math
from typing import Any

PROTOCOL_VERSION = 1
MAX_FRAME_BYTES = 256
MAX_DRIVE_TTL_MS = 350


class ProtocolError(ValueError):
    """Raised when a UART frame is invalid or unsafe."""


def encode_message(seq: int, name: str, **fields: Any) -> bytes:
    if isinstance(seq, bool) or not isinstance(seq, int) or seq < 0:
        raise ProtocolError("seq must be a non-negative integer")
    message = {"v": PROTOCOL_VERSION, "seq": seq, "type": "cmd", "name": name}
    message.update(fields)
    try:
        frame = (json.dumps(message, separators=(",", ":"), ensure_ascii=True) + "\n").encode("ascii")
    except (TypeError, ValueError) as exc:
        raise ProtocolError("message contains unsupported values") from exc
    if len(frame) > MAX_FRAME_BYTES:
        raise ProtocolError("frame exceeds maximum size")
    return frame


def drive_frame(seq: int, left: float, right: float, ttl_ms: int = 200) -> bytes:
    for value in (left, right):
        if (
            isinstance(value, bool)
            or not isinstance(value, (int, float))
            or not math.isfinite(value)
            or not -1.0 <= value <= 1.0
        ):
            raise ProtocolError("wheel speed must be finite and between -1.0 and 1.0")
    if isinstance(ttl_ms, bool) or not isinstance(ttl_ms, int) or not 1 <= ttl_ms <= MAX_DRIVE_TTL_MS:
        raise ProtocolError("ttl_ms outside safe range")
    return encode_message(seq, "drive", left=float(left), right=float(right), ttl_ms=ttl_ms)


def stop_frame(seq: int) -> bytes:
    return encode_message(seq, "stop")


def parse_response(frame: bytes) -> dict[str, Any]:
    if len(frame) > MAX_FRAME_BYTES:
        raise ProtocolError("response exceeds maximum size")
    try:
        payload = json.loads(frame.decode("utf-8"))
    except (UnicodeDecodeError, json.JSONDecodeError) as exc:
        raise ProtocolError("invalid response JSON") from exc
    if not isinstance(payload, dict) or payload.get("v") != PROTOCOL_VERSION:
        raise ProtocolError("unsupported response protocol")
    if not isinstance(payload.get("type"), str):
        raise ProtocolError("response type missing")
    return payload
