/*
 * Copyright (c) 2013-2019 Huawei Technologies Co., Ltd. All rights reserved.
 * Copyright (c) 2020-2021 Huawei Device Co., Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "it_test_process.h"

// This testcase us used for undefination of LOSCFG_USER_TEST_SMP
static int Testcase(void)
{
    int ret;
    int pid;
    int status;

    int temp = GetCpuCount();
    if (temp != 1) {
        return 0;
    }

    ret = fork();
    if (ret == 0) {
        int temp;
        cpu_set_t cpuset;

        CPU_ZERO(&cpuset);
        temp = sched_getaffinity(getpid(), sizeof(cpu_set_t), &cpuset);
        ICUNIT_GOTO_EQUAL(temp, 0, temp, ERROR_OUT);

        temp = CPU_COUNT(&cpuset);
        ICUNIT_GOTO_EQUAL(temp, 1, temp, ERROR_OUT); /* one cpu with the undefination of LOSCFG_USER_TEST_SMP */

ERROR_OUT:
        exit(11); // 11, exit args
    } else if (ret > 0) {
        pid = ret;
        ret = waitpid(ret, &status, 0);
        status = WEXITSTATUS(status);
        ICUNIT_ASSERT_EQUAL(ret, pid, ret);
        ICUNIT_ASSERT_EQUAL(status, 11, status); // 11, assert that function Result is equal to this.
    }

    return 0;
}

void ItTestProcess047(void)
{
    TEST_ADD_CASE("IT_POSIX_PROCESS_047", Testcase, TEST_POSIX, TEST_MEM, TEST_LEVEL0, TEST_FUNCTION);
}
