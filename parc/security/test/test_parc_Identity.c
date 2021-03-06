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
/**
 * @header <#Headline Name#>
 * @abstract <#Abstract#>
 * @discussion
 *     <#Discussion#>
 *
 * @author Scott, Glenn <Glenn.Scott@parc.com>, Palo Alto Research Center (Xerox PARC)
 * @copyright (c) 2014, Xerox Corporation (Xerox) and Palo Alto Research Center, Inc (PARC).  All rights reserved.
 */
// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_Identity.c"
#include "../parc_IdentityFile.h"
#include "../parc_Security.h"

#include <stdio.h>
#include <fcntl.h>
#include <inttypes.h>

#include <LongBow/unit-test.h>

#include <parc/algol/parc_SafeMemory.h>
#include <parc/testing/parc_ObjectTesting.h>

LONGBOW_TEST_RUNNER(test_parc_Identity)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(test_parc_Identity)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(test_parc_Identity)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcIdentity_Create);
    LONGBOW_RUN_TEST_CASE(Global, parcIdentity_Acquire);
    LONGBOW_RUN_TEST_CASE(Global, parcIdentity_GetFileName);
    LONGBOW_RUN_TEST_CASE(Global, parcIdentity_GetPassWord);
    LONGBOW_RUN_TEST_CASE(Global, parcIdentity_CreateSigner);
    LONGBOW_RUN_TEST_CASE(Global, parcIdentity_Equals);
    LONGBOW_RUN_TEST_CASE(Global, parcIdentity_Display);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    uint32_t outstandingAllocations = parcSafeMemory_ReportAllocation(STDERR_FILENO);
    if (outstandingAllocations != 0) {
        printf("Tests leak memory by %d allocations\n", outstandingAllocations);
        return LONGBOW_STATUS_MEMORYLEAK;
    }
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcIdentity_Create)
{
    const char *keystoreName = "test_rsa.p12";
    const char *keystorePassword = "blueberry";

    PARCIdentityFile *identityFile = parcIdentityFile_Create(keystoreName, keystorePassword);
    assertNotNull(identityFile, "Expected non-null");

    PARCIdentity *identity = parcIdentity_Create(identityFile, PARCIdentityFileAsPARCIdentity);
    assertNotNull(identity, "Expected non-null");
    parcIdentityFile_Release(&identityFile);

    parcIdentity_Release(&identity);
}

LONGBOW_TEST_CASE(Global, parcIdentity_Acquire)
{
    const char *keystoreName = "test_rsa.p12";
    const char *keystorePassword = "blueberry";

    PARCIdentityFile *identityFile = parcIdentityFile_Create(keystoreName, keystorePassword);
    assertNotNull(identityFile, "Expected non-null");

    PARCIdentity *identity = parcIdentity_Create(identityFile, PARCIdentityFileAsPARCIdentity);
    assertNotNull(identity, "Expected non-null");

    parcObjectTesting_AssertAcquireReleaseContract(parcIdentity_Acquire, identity);
   
    parcIdentityFile_Release(&identityFile);
    parcIdentity_Release(&identity);
}

LONGBOW_TEST_CASE(Global, parcIdentity_GetFileName)
{
    const char *keystoreName = "test_rsa.p12";
    const char *keystorePassword = "blueberry";

    PARCIdentityFile *identityFile = parcIdentityFile_Create(keystoreName, keystorePassword);
    assertNotNull(identityFile, "Expected non-null");

    PARCIdentity *identity = parcIdentity_Create(identityFile, PARCIdentityFileAsPARCIdentity);
    assertNotNull(identity, "Expected non-null");

    assertEqualStrings(keystoreName, parcIdentity_GetFileName(identity));
   
    parcIdentityFile_Release(&identityFile);
    parcIdentity_Release(&identity);
}

LONGBOW_TEST_CASE(Global, parcIdentity_GetPassWord)
{
    const char *keystoreName = "test_rsa.p12";
    const char *keystorePassword = "blueberry";

    PARCIdentityFile *identityFile = parcIdentityFile_Create(keystoreName, keystorePassword);
    assertNotNull(identityFile, "Expected non-null");

    PARCIdentity *identity = parcIdentity_Create(identityFile, PARCIdentityFileAsPARCIdentity);
    assertNotNull(identity, "Expected non-null");

    assertEqualStrings(keystorePassword, parcIdentity_GetPassWord(identity));
   
    parcIdentityFile_Release(&identityFile);
    parcIdentity_Release(&identity);
}

LONGBOW_TEST_CASE(Global, parcIdentity_CreateSigner)
{
    parcSecurity_Init();

    const char *keystoreName = "test_rsa.p12";
    const char *keystorePassword = "blueberry";

    PARCIdentityFile *identityFile = parcIdentityFile_Create(keystoreName, keystorePassword);
    assertNotNull(identityFile, "Expected non-null");

    PARCIdentity *identity = parcIdentity_Create(identityFile, PARCIdentityFileAsPARCIdentity);
    assertNotNull(identity, "Expected non-null");

    assertEqualStrings(keystorePassword, parcIdentity_GetPassWord(identity));

    PARCSigner *signer = parcIdentity_CreateSigner(identity);

    assertNotNull(signer, "Expected non-null");
   
    parcIdentityFile_Release(&identityFile);
    parcIdentity_Release(&identity);
    parcSigner_Release(&signer);

    parcSecurity_Fini();
}

LONGBOW_TEST_CASE(Global, parcIdentity_Equals)
{
    const char *keystoreName = "test_rsa.p12";
    const char *keystorePassword = "blueberry";

    PARCIdentityFile *identityFileX = parcIdentityFile_Create(keystoreName, keystorePassword);
    PARCIdentity *x = parcIdentity_Create(identityFileX, PARCIdentityFileAsPARCIdentity);
    PARCIdentityFile *identityFileY = parcIdentityFile_Create(keystoreName, keystorePassword);
    PARCIdentity *y = parcIdentity_Create(identityFileY, PARCIdentityFileAsPARCIdentity);
    PARCIdentityFile *identityFileZ = parcIdentityFile_Create(keystoreName, keystorePassword);
    PARCIdentity *z = parcIdentity_Create(identityFileZ, PARCIdentityFileAsPARCIdentity);

    PARCIdentityFile *identityFile1 = parcIdentityFile_Create("foo", keystorePassword);
    PARCIdentityFile *identityFile2 = parcIdentityFile_Create(keystoreName, "bar");
    PARCIdentity *u1 = parcIdentity_Create(identityFile1, PARCIdentityFileAsPARCIdentity);
    PARCIdentity *u2 = parcIdentity_Create(identityFile2, PARCIdentityFileAsPARCIdentity);

    parcObjectTesting_AssertEqualsFunction(parcIdentity_Equals, x, y, z, u1, u2);
   
    parcIdentityFile_Release(&identityFileX);
    parcIdentityFile_Release(&identityFileY);
    parcIdentityFile_Release(&identityFileZ);
    parcIdentityFile_Release(&identityFile1);
    parcIdentityFile_Release(&identityFile2);
   
    parcIdentity_Release(&x);
    parcIdentity_Release(&y);
    parcIdentity_Release(&z);
    parcIdentity_Release(&u1);
    parcIdentity_Release(&u2);
}

LONGBOW_TEST_CASE(Global, parcIdentity_Display)
{
    const char *keystoreName = "test_rsa.p12";
    const char *keystorePassword = "blueberry";

    PARCIdentityFile *identityFile = parcIdentityFile_Create(keystoreName, keystorePassword);
    assertNotNull(identityFile, "Expected non-null");

    PARCIdentity *identity = parcIdentity_Create(identityFile, PARCIdentityFileAsPARCIdentity);
    assertNotNull(identity, "Expected non-null");

    parcIdentity_Display(identity, 0);
   
    parcIdentityFile_Release(&identityFile);
    parcIdentity_Release(&identity);
}

int
main(int argc, char *argv[])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(test_parc_Identity);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
