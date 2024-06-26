/* cryptodto/params.h
 *
 * This file is part of AFV-Native.
 *
 * Copyright (c) 2019 Christopher Collins
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef AFV_NATIVE_CRYPTODTO_PARAMS_H
#define AFV_NATIVE_CRYPTODTO_PARAMS_H

#include <cstdint>

namespace afv_native { namespace cryptodto {
    typedef uint64_t sequence_t;

    typedef uint64_t sequence_bitfield_t;

    // sizes are all in bytes (octets)
    const int aeadModeKeySize = 32;

    const int aeadModeIVSize = 12;

    const int aeadModeTagSize = 16;

    // maximum size we permit for an on-wire DTO.
    // this sets the upper limit and buffering
    // size requirements for our receive handler.
    const int maxPermittedDatagramSize = 65536;

    enum CryptoDtoMode {
        CryptoModeUndefined        = 0,
        CryptoModeNone             = 1,
        CryptoModeChaCha20Poly1305 = 2,
        CryptoModeLast
    };
}}     // namespace afv_native::cryptodto
#endif // AFV_NATIVE_CRYPTODTO_PARAMS_H
