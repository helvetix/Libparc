/*
 * Copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX OR PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ################################################################################
 * #
 * # PATENT NOTICE
 * #
 * # This software is distributed under the BSD 2-clause License (see LICENSE
 * # file).  This BSD License does not make any patent claims and as such, does
 * # not act as a patent grant.  The purpose of this section is for each contributor
 * # to define their intentions with respect to intellectual property.
 * #
 * # Each contributor to this source code is encouraged to state their patent
 * # claims and licensing mechanisms for any contributions made. At the end of
 * # this section contributors may each make their own statements.  Contributor's
 * # claims and grants only apply to the pieces (source code, programs, text,
 * # media, etc) that they have contributed directly to this software.
 * #
 * # There is no guarantee that this section is complete, up to date or accurate. It
 * # is up to the contributors to maintain their portion of this section and up to
 * # the user of the software to verify any claims herein.
 * #
 * # Do not remove this header notification.  The contents of this section must be
 * # present in all distributions of the software.  You may only modify your own
 * # intellectual property statements.  Please provide contact information.
 *
 * - Palo Alto Research Center, Inc
 * This software distribution does not grant any rights to patents owned by Palo
 * Alto Research Center, Inc (PARC). Rights to these patents are available via
 * various mechanisms. As of January 2016 PARC has committed to FRAND licensing any
 * intellectual property used by its contributions to this software. You may
 * contact PARC at cipo@parc.com for more information or visit http://www.ccnx.org
 */
/**
 * @author Glenn Scott, Palo Alto Research Center (PARC)
 * @copyright (c) 2016, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
#include "../parc_BufferPool.c"

#include <LongBow/testing.h>
#include <LongBow/debugging.h>
#include <parc/algol/parc_Memory.h>
#include <parc/algol/parc_SafeMemory.h>
#include <parc/algol/parc_DisplayIndented.h>

#include <parc/testing/parc_MemoryTesting.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(parc_BufferPool)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(CreateAcquireRelease);
    LONGBOW_RUN_TEST_FIXTURE(Object);
    LONGBOW_RUN_TEST_FIXTURE(Specialization);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_BufferPool)
{
    parcMemory_SetInterface(&PARCSafeMemoryAsPARCMemory);
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_BufferPool)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(CreateAcquireRelease)
{
    LONGBOW_RUN_TEST_CASE(CreateAcquireRelease, CreateRelease);
}

LONGBOW_TEST_FIXTURE_SETUP(CreateAcquireRelease)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(CreateAcquireRelease)
{
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s leaked memory.", longBowTestCase_GetFullName(testCase))) {
        parcSafeMemory_ReportAllocation(1);
        return LONGBOW_STATUS_MEMORYLEAK;
    }

    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(CreateAcquireRelease, CreateRelease)
{
    PARCBufferPool *instance = parcBufferPool_Create(3, 10);
    assertNotNull(instance, "Expected non-null result from parcBufferPool_Create();");

    parcObjectTesting_AssertAcquireReleaseContract(parcBufferPool_Acquire, instance);
   
    parcBufferPool_Release(&instance);
    assertNull(instance, "Expected null result from parcBufferPool_Release();");
}

LONGBOW_TEST_FIXTURE(Object)
{
    LONGBOW_RUN_TEST_CASE(Object, parcBufferPool_Display);
    LONGBOW_RUN_TEST_CASE(Object, parcBufferPool_IsValid);
    LONGBOW_RUN_TEST_CASE(Object, parcBufferPool_AssertValid);
}

LONGBOW_TEST_FIXTURE_SETUP(Object)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Object)
{
    if (!parcMemoryTesting_ExpectedOutstanding(0, "%s mismanaged memory.", longBowTestCase_GetFullName(testCase))) {
        return LONGBOW_STATUS_MEMORYLEAK;
    }
   
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Object, parcBufferPool_Display)
{
    PARCBufferPool *instance = parcBufferPool_Create(3, 10);
    parcBufferPool_Display(instance, 0);
    parcBufferPool_Release(&instance);
}

LONGBOW_TEST_CASE(Object, parcBufferPool_IsValid)
{
    PARCBufferPool *instance = parcBufferPool_Create(3, 10);
    assertTrue(parcBufferPool_IsValid(instance), "Expected parcBufferPool_Create to result in a valid instance.");
   
    parcBufferPool_Release(&instance);
    assertFalse(parcBufferPool_IsValid(instance), "Expected parcBufferPool_Release to result in an invalid instance.");
}

LONGBOW_TEST_CASE(Object, parcBufferPool_AssertValid)
{
    PARCBufferPool *instance = parcBufferPool_Create(3, 10);
    parcBufferPool_AssertValid(instance);
   
    parcBufferPool_Release(&instance);
    assertFalse(parcBufferPool_IsValid(instance), "Expected parcBufferPool_Release to result in an invalid instance.");
}

LONGBOW_TEST_FIXTURE(Specialization)
{
    LONGBOW_RUN_TEST_CASE(Specialization, parcBufferPool_GetInstance);
    LONGBOW_RUN_TEST_CASE(Specialization, parcBufferPool_GetLargestPoolSize);
    LONGBOW_RUN_TEST_CASE(Specialization, parcBufferPool_GetCurrentPoolSize);
    LONGBOW_RUN_TEST_CASE(Specialization, parcBufferPool_GetTotalInstances);
    LONGBOW_RUN_TEST_CASE(Specialization, parcBufferPool_GetCacheHits);
    LONGBOW_RUN_TEST_CASE(Specialization, parcBufferPool_GetLimit);
    LONGBOW_RUN_TEST_CASE(Specialization, parcBufferPool_SetLimit_Increasing);
    LONGBOW_RUN_TEST_CASE(Specialization, parcBufferPool_SetLimit_Decreasing);
    LONGBOW_RUN_TEST_CASE(Specialization, parcBufferPool_Drain);
}

LONGBOW_TEST_FIXTURE_SETUP(Specialization)
{
    longBowTestCase_SetInt(testCase, "initialAllocations", parcMemory_Outstanding());
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Specialization)
{
    int initialAllocations = longBowTestCase_GetInt(testCase, "initialAllocations");
   
    if (parcMemory_Outstanding() > initialAllocations) {
        parcSafeMemory_ReportAllocation(1);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
   
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Specialization, parcBufferPool_GetInstance)
{
    PARCBufferPool *pool = parcBufferPool_Create(3, 10);
   
    PARCBuffer *buffer = parcBufferPool_GetInstance(pool);
   
    parcBuffer_AssertValid(buffer);
    parcBuffer_Release(&buffer);
   
    size_t largestPoolSize = parcBufferPool_GetLargestPoolSize(pool);
   
    assertTrue(largestPoolSize == 1, "Expected the largestPoolSize to be 1, actual %zu", largestPoolSize);
   
    parcBufferPool_Release(&pool);
}

LONGBOW_TEST_CASE(Specialization, parcBufferPool_GetLargestPoolSize)
{
    PARCBufferPool *pool = parcBufferPool_Create(3, 10);
    size_t largestPoolSize = parcBufferPool_GetLargestPoolSize(pool);
   
    assertTrue(largestPoolSize == 0, "Expected the largestPoolSize to be 0, actual %zu", largestPoolSize);
   
    PARCBuffer *buffer = parcBufferPool_GetInstance(pool);
   
    parcBuffer_AssertValid(buffer);
    parcBuffer_Release(&buffer);
   
    largestPoolSize = parcBufferPool_GetLargestPoolSize(pool);
   
    assertTrue(largestPoolSize == 1, "Expected the largestPoolSize to be 1, actual %zu", largestPoolSize);
   
    parcBufferPool_Release(&pool);
}

LONGBOW_TEST_CASE(Specialization, parcBufferPool_GetTotalInstances)
{
    PARCBufferPool *pool = parcBufferPool_Create(3, 10);
    size_t totalInstances = parcBufferPool_GetTotalInstances(pool);
   
    assertTrue(totalInstances == 0, "Expected the totalInstances to be 0, actual %zu", totalInstances);
   
    PARCBuffer *buffer = parcBufferPool_GetInstance(pool);
   
    parcBuffer_AssertValid(buffer);
    parcBuffer_Release(&buffer);
   
    totalInstances = parcBufferPool_GetTotalInstances(pool);
   
    assertTrue(totalInstances == 1, "Expected the totalInstances to be 1, actual %zu", totalInstances);
   
    parcBufferPool_Release(&pool);
}

LONGBOW_TEST_CASE(Specialization, parcBufferPool_GetCacheHits)
{
    PARCBufferPool *pool = parcBufferPool_Create(3, 10);
    size_t cacheHits = parcBufferPool_GetCacheHits(pool);
   
    assertTrue(cacheHits == 0, "Expected the cacheHits to be 0, actual %zu", cacheHits);
   
    PARCBuffer *buffer = parcBufferPool_GetInstance(pool);
    parcBuffer_AssertValid(buffer);
    parcBuffer_Release(&buffer);

    cacheHits = parcBufferPool_GetCacheHits(pool);
    assertTrue(cacheHits == 0, "Expected the cacheHits to be 0, actual %zu", cacheHits);
   
    buffer = parcBufferPool_GetInstance(pool);
    parcBuffer_AssertValid(buffer);
    parcBuffer_Release(&buffer);
   
    cacheHits = parcBufferPool_GetCacheHits(pool);
    assertTrue(cacheHits == 1, "Expected the cacheHits to be 1, actual %zu", cacheHits);
   
    parcBufferPool_Release(&pool);
}

LONGBOW_TEST_CASE(Specialization, parcBufferPool_GetLimit)
{
    size_t expected = 20;
   
    PARCBufferPool *pool = parcBufferPool_Create(expected, 10);
    size_t limit = parcBufferPool_GetLimit(pool);
   
    assertTrue(limit == expected, "Expected the limit to be %zu, actual %zu", expected, limit);
    parcBufferPool_Release(&pool);
}

LONGBOW_TEST_CASE(Specialization, parcBufferPool_GetCurrentPoolSize)
{
    size_t expectedLimit = 3;
   
    PARCBufferPool *pool = parcBufferPool_Create(expectedLimit, 10);
    size_t poolSize = parcBufferPool_GetCurrentPoolSize(pool);
   
    assertTrue(poolSize == 0, "Expected the poolSize to be 0, actual %zu", poolSize);
   
    PARCBuffer *buffer1 = parcBufferPool_GetInstance(pool);
    PARCBuffer *buffer2 = parcBufferPool_GetInstance(pool);
    PARCBuffer *buffer3 = parcBufferPool_GetInstance(pool);
    PARCBuffer *buffer4 = parcBufferPool_GetInstance(pool);
    PARCBuffer *buffer5 = parcBufferPool_GetInstance(pool);

    parcBuffer_Release(&buffer1);
    parcBuffer_Release(&buffer2);
    parcBuffer_Release(&buffer3);
    parcBuffer_Release(&buffer4);
    parcBuffer_Release(&buffer5);
   
    poolSize = parcBufferPool_GetCurrentPoolSize(pool);
   
    assertTrue(poolSize == expectedLimit, "Expected the poolSize to be %zu, actual %zu", expectedLimit, poolSize);
   
    parcBufferPool_Release(&pool);
}

LONGBOW_TEST_CASE(Specialization, parcBufferPool_SetLimit_Increasing)
{
    size_t oldLimit = 3;
    size_t newLimit = 5;
   
    PARCBufferPool *pool = parcBufferPool_Create(oldLimit, 10);
    size_t limit = parcBufferPool_GetLimit(pool);
   
    assertTrue(limit == oldLimit, "Expected the limit to be %zu, actual %zu", oldLimit, limit);
   
    PARCBuffer *buffer1 = parcBufferPool_GetInstance(pool);
    PARCBuffer *buffer2 = parcBufferPool_GetInstance(pool);
    PARCBuffer *buffer3 = parcBufferPool_GetInstance(pool);
    parcBuffer_AssertValid(buffer1);
    parcBuffer_AssertValid(buffer2);
    parcBuffer_AssertValid(buffer3);
    parcBuffer_Release(&buffer1);
    parcBuffer_Release(&buffer2);
    parcBuffer_Release(&buffer3);
   
    limit = parcBufferPool_SetLimit(pool, newLimit);
    assertTrue(limit == 3, "Expected the old limit to be %zu, actual %zu", oldLimit, limit);
   
    size_t largestPoolSize = parcBufferPool_GetLargestPoolSize(pool);
    assertTrue(largestPoolSize == oldLimit, "Expected largest pool size to be %zu, actual %zu", oldLimit, largestPoolSize);
   
   
    parcBufferPool_Release(&pool);
}

LONGBOW_TEST_CASE(Specialization, parcBufferPool_SetLimit_Decreasing)
{
    size_t oldLimit = 3;
    size_t newLimit = 2;
   
    PARCBufferPool *pool = parcBufferPool_Create(oldLimit, 10);
    size_t limit = parcBufferPool_GetLimit(pool);
   
    assertTrue(limit == oldLimit, "Expected the limit to be %zu, actual %zu", oldLimit, limit);
   
    PARCBuffer *buffer1 = parcBufferPool_GetInstance(pool);
    PARCBuffer *buffer2 = parcBufferPool_GetInstance(pool);
    PARCBuffer *buffer3 = parcBufferPool_GetInstance(pool);
    parcBuffer_AssertValid(buffer1);
    parcBuffer_AssertValid(buffer2);
    parcBuffer_AssertValid(buffer3);
    parcBuffer_Release(&buffer1);
    parcBuffer_Release(&buffer2);
    parcBuffer_Release(&buffer3);
   
    limit = parcBufferPool_SetLimit(pool, newLimit);
    assertTrue(limit == 3, "Expected the old limit to be %zu, actual %zu", oldLimit, limit);
   
    size_t largestPoolSize = parcBufferPool_GetLargestPoolSize(pool);
    assertTrue(largestPoolSize == oldLimit, "Expected largest pool size to be %zu, actual %zu", oldLimit, largestPoolSize);
   
   
    parcBufferPool_Release(&pool);
}

LONGBOW_TEST_CASE(Specialization, parcBufferPool_Drain)
{
    size_t oldLimit = 3;
    size_t newLimit = 2;
   
    PARCBufferPool *pool = parcBufferPool_Create(oldLimit, 10);
   
    PARCBuffer *buffer1 = parcBufferPool_GetInstance(pool);
    PARCBuffer *buffer2 = parcBufferPool_GetInstance(pool);
    PARCBuffer *buffer3 = parcBufferPool_GetInstance(pool);
    parcBuffer_AssertValid(buffer1);
    parcBuffer_AssertValid(buffer2);
    parcBuffer_AssertValid(buffer3);
    parcBuffer_Release(&buffer1);
    parcBuffer_Release(&buffer2);
    parcBuffer_Release(&buffer3);
   
    size_t limit = parcBufferPool_SetLimit(pool, newLimit);
    assertTrue(limit == oldLimit, "Expected the limit to be %zu, actual %zu", oldLimit, limit);
   
    size_t drained = parcBufferPool_Drain(pool);
    assertTrue(drained == 1, "Expected the drained to be 1, actual %zu", drained);
   
    parcBufferPool_Release(&pool);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_BufferPool);
    int exitStatus = longBowMain(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
