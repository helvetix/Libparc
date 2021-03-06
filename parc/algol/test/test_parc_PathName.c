/*
 * Copyright (c) 2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC)
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
/*
 *
 */
#include "../parc_PathName.c"
#include <LongBow/unit-test.h>

// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_SafeMemory.h"
#include <parc/testing/parc_ObjectTesting.h>

#define PATH_SEGMENT "A"

LONGBOW_TEST_RUNNER(parc_PathName)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Local);
    LONGBOW_RUN_TEST_FIXTURE(AcquireRelease);
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(parc_PathName)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(parc_PathName)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(AcquireRelease)
{
    LONGBOW_RUN_TEST_CASE(AcquireRelease, parcPathName_Create);
    LONGBOW_RUN_TEST_CASE(AcquireRelease, parcPathName_Release);
    LONGBOW_RUN_TEST_CASE(AcquireRelease, parcPathName_AcquireRelease);
}

LONGBOW_TEST_FIXTURE_SETUP(AcquireRelease)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(AcquireRelease)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDOUT_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(AcquireRelease, parcPathName_Create)
{
    PARCPathName *pathName = parcPathName_Create();
    assertNotNull(pathName, "Expected a non-null pointer");

    parcPathName_Release(&pathName);
    assertNull(pathName, "Expected parcPathName_Release to null the pointer");
}

LONGBOW_TEST_CASE(AcquireRelease, parcPathName_Release)
{
    PARCPathName *pathName = parcPathName_Create();
    assertNotNull(pathName, "Expected a non-null pointer");

    parcPathName_Release(&pathName);
    assertNull(pathName, "Expected parcPathName_Release to null the pointer");
}

LONGBOW_TEST_CASE(AcquireRelease, parcPathName_AcquireRelease)
{
    PARCPathName *original = parcPathName_Create();
    assertNotNull(original, "Expected non-null result from parcPathName_Create()");

    PARCPathName *reference = parcPathName_Acquire(original);
    assertTrue(original == reference, "Expected the reference to be equal to the original.");

    parcPathName_Release(&original);
    assertNull(original, "Expected parcDeque_Release to null the pointer");

    parcPathName_Append(reference, (void *) "Hello");
    size_t expected = 1;
    size_t actual = parcPathName_Size(reference);
    assertTrue(expected == actual,
               "Expected size %zd, actual %zd", expected, actual);
    parcPathName_Release(&reference);
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcPathName_Create);

    LONGBOW_RUN_TEST_CASE(Global, parcPathName_Size);
    LONGBOW_RUN_TEST_CASE(Global, parcPathName_Append);
    LONGBOW_RUN_TEST_CASE(Global, parcPathName_Prepend);
    LONGBOW_RUN_TEST_CASE(Global, parcPathName_IsAbsolute);
    LONGBOW_RUN_TEST_CASE(Global, parcPathName_MakeAbsolute);
    LONGBOW_RUN_TEST_CASE(Global, parcPathName_Parse_AbsolutePath);
    LONGBOW_RUN_TEST_CASE(Global, parcPathName_Parse_AbsolutePath_Limited);
    LONGBOW_RUN_TEST_CASE(Global, parcPathName_Parse_RelativePath);
    LONGBOW_RUN_TEST_CASE(Global, parcPathName_ToString_AbsolutePath);
    LONGBOW_RUN_TEST_CASE(Global, parcPathName_ToString_RelativePath);
    LONGBOW_RUN_TEST_CASE(Global, parcPathName_Head);
    LONGBOW_RUN_TEST_CASE(Global, parcPathName_Tail);
    LONGBOW_RUN_TEST_CASE(Global, parcPathName_Tail_ExceedsLength);
    LONGBOW_RUN_TEST_CASE(Global, parcPathName_Equals);
    LONGBOW_RUN_TEST_CASE(Global, parcPathName_Copy);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDOUT_FILENO);
    if (outstandingAllocations != 0) {
        printf("%s leaks memory by %d allocations\n", longBowTestCase_GetName(testCase), outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcPathName_Size)
{
    char *path = "/a/b/c";
    PARCPathName *pathName = parcPathName_Parse(path);
    assertNotNull(pathName, "Expected a non-null pointer");
    assertTrue(pathName->isAbsolute, "Expected the PARCPathName to be absolute.");

    assertTrue(parcPathName_Size(pathName) == 3, "Expected 3, actual %zu", parcPathName_Size(pathName));
    parcPathName_Release(&pathName);
}

LONGBOW_TEST_CASE(Global, parcPathName_Prepend)
{
    PARCPathName *pathName = parcPathName_Create();
    size_t size = 1000;

    char expected[10];
    for (size_t i = 0; i < size; i++) {
        sprintf(expected, "%zd", i);
        parcPathName_Prepend(pathName, expected);
    }
    assertNotNull(pathName, "Expected a non-null pointer");

    size_t actual = parcPathName_Size(pathName);
    assertTrue(size == actual,
               "Expected %zd, actual %zd", size, actual);

    for (size_t i = 0; i < size; i++) {
        sprintf(expected, "%zd", size - i - 1);
        char *segment = parcDeque_GetAtIndex(pathName->path, i);
        assertTrue(strcmp(segment, expected) == 0,
                   "Expected %s, actual %s", expected, segment);
    }

    parcPathName_Release(&pathName);
}

LONGBOW_TEST_CASE(Global, parcPathName_Append)
{
    PARCPathName *pathName = parcPathName_Create();
    size_t size = 1000;

    char expected[10];
    for (size_t i = 0; i < size; i++) {
        sprintf(expected, "%zd", i);
        parcPathName_Append(pathName, expected);
    }
    assertNotNull(pathName, "Expected a non-null pointer");

    size_t actual = parcPathName_Size(pathName);
    assertTrue(size == actual,
               "Expected %zd, actual %zd", size, actual);

    for (size_t i = 0; i < size; i++) {
        sprintf(expected, "%zd", i);
        char *segment = parcDeque_GetAtIndex(pathName->path, i);
        assertTrue(strcmp(segment, expected) == 0,
                   "Expected %s, actual %s", expected, segment);
    }

    parcPathName_Release(&pathName);
}

LONGBOW_TEST_CASE(Global, parcPathName_Create)
{
    PARCPathName *pathName = parcPathName_Create();
    assertNotNull(pathName, "Expected a non-null pointer");

    parcPathName_Release(&pathName);
    assertNull(pathName, "Expected parcPathName_Release to null the pointer");
}

LONGBOW_TEST_CASE(Global, parcPathName_IsAbsolute)
{
    char *path = "/a/b/c/d/e/f/g/h/i/j/k/l/m/n/o/p/q/r/s/t/u/v/w/x/y/z/aa/bb/cc/dd/ee/ff/gg/hh/ii/jj/kk/ll/mm/nn/oo/pp/qq/rr/ss/tt/uu/vv/ww/xx/yy/zz";
    PARCPathName *pathName = parcPathName_Parse(path);
    assertNotNull(pathName, "Expected a non-null pointer");
    assertTrue(parcPathName_IsAbsolute(pathName), "Expected the PARCPathName to be absolute.");

    parcPathName_Release(&pathName);
}

LONGBOW_TEST_CASE(Global, parcPathName_MakeAbsolute)
{
#define PATH "a/b/c/d/e/f/g/h/i/j/k/l/m/n/o/p/q/r/s/t/u/v/w/x/y/z"
    char *expected = PATH;
    PARCPathName *pathName = parcPathName_Parse(expected);
    assertNotNull(pathName, "Expected a non-null pointer");
    assertFalse(pathName->isAbsolute, "Expected the PARCPathName to be relative.");

    parcPathName_MakeAbsolute(pathName, true);

    char *actual = parcPathName_ToString(pathName);
    assertTrue(strcmp("/" PATH, actual) == 0,
               "Expected '%s' actual '%s'", expected, actual);
    parcMemory_Deallocate((void **) &actual);
    parcPathName_Release(&pathName);
}

LONGBOW_TEST_CASE(Global, parcPathName_Parse_AbsolutePath)
{
    PARCBufferComposer *composer = parcBufferComposer_Create();
    for (int i = 0; i < 1000; i++) {
        parcBufferComposer_Format(composer, "/%d", i);
    }

    PARCBuffer *tempBuffer = parcBufferComposer_ProduceBuffer(composer);
    char *path = parcBuffer_ToString(tempBuffer);
    parcBuffer_Release(&tempBuffer);

    PARCPathName *pathName = parcPathName_Parse(path);
    assertNotNull(pathName, "Expected a non-null pointer");
    assertTrue(pathName->isAbsolute, "Expected the PARCPathName to be absolute.");


    char *actual = parcPathName_ToString(pathName);
    assertTrue(strcmp(path, actual) == 0, "Expected %s, actual %s", path, actual);

    parcMemory_Deallocate((void **) &actual);
    parcMemory_Deallocate((void **) &path);

    parcPathName_Release(&pathName);
    parcBufferComposer_Release(&composer);
}

LONGBOW_TEST_CASE(Global, parcPathName_Parse_AbsolutePath_Limited)
{
    PARCBufferComposer *composer = parcBufferComposer_Create();
    for (int i = 0; i < 10; i++) {
        parcBufferComposer_Format(composer, "/%d", i);
    }
    parcBufferComposer_Format(composer, "?hello world");

    PARCBuffer *tempBuffer = parcBufferComposer_ProduceBuffer(composer);
    char *path = parcBuffer_ToString(tempBuffer);
    parcBuffer_Release(&tempBuffer);

    size_t limit = strchr(path, '?') - path;
    PARCPathName *pathName = parcPathName_ParseToLimit(limit, path);

    assertNotNull(pathName, "Expected a non-null pointer");
    assertTrue(pathName->isAbsolute, "Expected the PARCPathName to be absolute.");

    path[limit] = 0;
    char *actual = parcPathName_ToString(pathName);
    assertTrue(strcmp(path, actual) == 0, "Expected %s, actual %s", path, actual);

    parcMemory_Deallocate((void **) &actual);
    parcMemory_Deallocate((void **) &path);

    parcPathName_Release(&pathName);
    parcBufferComposer_Release(&composer);
}

LONGBOW_TEST_CASE(Global, parcPathName_Parse_RelativePath)
{
    PARCBufferComposer *composer = parcBufferComposer_Create();
    for (int i = 0; i < 1000; i++) {
        parcBufferComposer_Format(composer, "%d/", i);
    }
    parcBufferComposer_Format(composer, "%d", 1000);

    PARCBuffer *tempBuffer = parcBufferComposer_ProduceBuffer(composer);
    char *expected = parcBuffer_ToString(tempBuffer);
    parcBuffer_Release(&tempBuffer);

    PARCPathName *pathName = parcPathName_Parse(expected);
    assertNotNull(pathName, "Expected a non-null pointer");
    assertFalse(pathName->isAbsolute, "Expected the PARCPathName to be relative.");

    char *actual = parcPathName_ToString(pathName);
    assertTrue(strcmp(expected, actual) == 0,
               "Expected '%s' actual '%s'", expected, actual);
    parcMemory_Deallocate((void **) &actual);
    parcMemory_Deallocate((void **) &expected);

    parcPathName_Release(&pathName);

    parcBufferComposer_Release(&composer);
}

LONGBOW_TEST_CASE(Global, parcPathName_ToString_AbsolutePath)
{
    char *path = "/a/b/c";
    PARCPathName *pathName = parcPathName_Parse(path);
    assertNotNull(pathName, "Expected a non-null pointer");
    assertTrue(pathName->isAbsolute, "Expected the PARCPathName to be absolute.");

    char *actual = parcPathName_ToString(pathName);

    assertTrue(strcmp(path, actual) == 0, "Expected '%s' actual '%s'", path, actual);

    parcMemory_Deallocate((void **) &actual);

    parcPathName_Release(&pathName);
}

LONGBOW_TEST_CASE(Global, parcPathName_ToString_RelativePath)
{
    char *path = "a/b/c";
    PARCPathName *pathName = parcPathName_Parse(path);
    assertNotNull(pathName, "Expected a non-null pointer");
    assertFalse(pathName->isAbsolute, "Expected the PARCPathName to be relative.");

    char *actual = parcPathName_ToString(pathName);

    assertTrue(strcmp(path, actual) == 0, "Expected '%s' actual '%s'", path, actual);

    parcMemory_Deallocate((void **) &actual);

    parcPathName_Release(&pathName);
}

LONGBOW_TEST_CASE(Global, parcPathName_Head)
{
    PARCPathName *original = parcPathName_Parse("/" PATH_SEGMENT "/" PATH_SEGMENT "/" PATH_SEGMENT "/" PATH_SEGMENT);
    assertNotNull(original, "Expected a non-null pointer");

    PARCPathName *expected = parcPathName_Parse("/" PATH_SEGMENT "/" PATH_SEGMENT "/" PATH_SEGMENT);
    PARCPathName *actual = parcPathName_Head(original, 3);

    assertTrue(parcPathName_Equals(expected, actual),
               "expected did not match actual");

    parcPathName_Release(&original);
    parcPathName_Release(&expected);
    parcPathName_Release(&actual);
}

LONGBOW_TEST_CASE(Global, parcPathName_Tail)
{
    PARCPathName *original = parcPathName_Parse("/" PATH_SEGMENT "/" PATH_SEGMENT "/" PATH_SEGMENT "/" PATH_SEGMENT "/" PATH_SEGMENT "/" PATH_SEGMENT);
    assertNotNull(original, "Expected a non-null pointer");

    PARCPathName *expected = parcPathName_Parse(PATH_SEGMENT "/" PATH_SEGMENT "/" PATH_SEGMENT);
    PARCPathName *actual = parcPathName_Tail(original, 3);

    assertTrue(parcPathName_Equals(expected, actual),
               "expected did not match actual");

    parcPathName_Release(&original);
    parcPathName_Release(&expected);
    parcPathName_Release(&actual);
}

LONGBOW_TEST_CASE(Global, parcPathName_Tail_ExceedsLength)
{
    PARCPathName *original = parcPathName_Parse("/" PATH_SEGMENT "/" PATH_SEGMENT "/" PATH_SEGMENT "/" PATH_SEGMENT "/" PATH_SEGMENT "/" PATH_SEGMENT);
    assertNotNull(original, "Expected a non-null pointer");

    PARCPathName *actual = parcPathName_Tail(original, 10000000);

    parcPathName_MakeAbsolute(original, false);

    assertTrue(parcPathName_Equals(original, actual),
               "expected did not match actual");

    parcPathName_Release(&original);
    parcPathName_Release(&actual);
}

LONGBOW_TEST_CASE(Global, parcPathName_Equals)
{
    PARCPathName *x = parcPathName_Parse("/a/b/c/d/");
    PARCPathName *y = parcPathName_Parse("/a/b/c/d/");
    PARCPathName *z = parcPathName_Parse("/a/b/c/d/");
    PARCPathName *u1 = parcPathName_Parse("/a/b/c/d/e");
    PARCPathName *u2 = parcPathName_Parse("/a/b/c/");
    PARCPathName *u3 = parcPathName_Parse("a/b/c/");

    parcObjectTesting_AssertEqualsFunction(parcPathName_Equals, x, y, z, u1, u2, u3, NULL);

    parcPathName_Release(&x);
    parcPathName_Release(&y);
    parcPathName_Release(&z);
    parcPathName_Release(&u1);
    parcPathName_Release(&u2);
    parcPathName_Release(&u3);
}

LONGBOW_TEST_CASE(Global, parcPathName_Copy)
{
    PARCPathName *x = parcPathName_Parse("/a/b/c/d/");
    PARCPathName *y = parcPathName_Copy(x);

    assertTrue(parcPathName_Equals(x, y), "Expected the copy to be equal to the original.");

    parcPathName_Release(&x);
    parcPathName_Release(&y);
}

LONGBOW_TEST_FIXTURE(Local)
{
}

LONGBOW_TEST_FIXTURE_SETUP(Local)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Local)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(parc_PathName);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
