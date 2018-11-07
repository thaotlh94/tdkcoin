// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "arith_uint256.h"
#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "TDKCoin is greater than all over the world";
    const CScript genesisOutputScript = CScript() << ParseHex("04fb113b0f20b597b2864c21e278ea76a1660d80175ccc896afe91d34caf898ccf31debeb930de7a78bba98f0f688e414ac44374c6828f2171abf3943f0d3f0bc2") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 2000000;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("aeee7a5642b9ea4c6c781d4f212efaab80d6ed20e37b705ef6622da8799a5548");
        consensus.BIP65Height = 0; // ?
        consensus.BIP66Height = 0; // ?
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); 
        consensus.nPowTargetTimespan = 10 * 60; // 10 min
        consensus.nPowTargetSpacing = 5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 6; // 75% of 8064
        consensus.nMinerConfirmationWindow = 8; // nPowTargetTimespan / nPowTargetSpacing * 4
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1485561600; // January 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517356801; // January 31st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1485561600; // January 28, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1517356801; // January 31st, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x0000000000000000000000000000000000000000000000000000000000400022");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x17a2cc16946ae28ad59f73fa247a3e1f028fef76ad477a363f9349f9b901aa54"); //3

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xd2;
        pchMessageStart[2] = 0xa3;
        pchMessageStart[3] = 0xc4;
        nDefaultPort = 32234;
        nPruneAfterHeight = 2000;

        genesis = CreateGenesisBlock(1541005692, 2248077, 0x1e0ffff0, 1, 5 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

/*
         consensus.hashGenesisBlock = uint256S("0x");
         std::cout << std::string("Begin calculating Mainnet Genesis Block:\n");
         if (true && (genesis.GetHash() != consensus.hashGenesisBlock)) {
             LogPrintf("Calculating Mainnet Genesis Block:\n");
             arith_uint256 hashTarget = arith_uint256().SetCompact(genesis.nBits);
             uint256 hash;
             genesis.nNonce = 0;
             // This will figure out a valid hash and Nonce if you're
             // creating a different genesis block:
             // uint256 hashTarget = CBigNum().SetCompact(genesis.nBits).getuint256();
             // hashTarget.SetCompact(genesis.nBits, &fNegative, &fOverflow).getuint256();
             // while (genesis.GetHash() > hashTarget)
             while (UintToArith256(genesis.GetPoWHash()) > hashTarget)
             {
                 ++genesis.nNonce;
				 std::cout << "  Current nonce: " << genesis.nNonce <<  "\n";
                 if (genesis.nNonce == 0)
                 {
                     LogPrintf("NONCE WRAPPED, incrementing time");
                     std::cout << std::string("NONCE WRAPPED, incrementing time:\n");
                     ++genesis.nTime;
                 }
                 if (genesis.nNonce % 10000 == 0)
                 {
                     LogPrintf("Mainnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
                     // std::cout << strNetworkID << " nonce: " << genesis.nNonce << " time: " << genesis.nTime << " hash: " << genesis.GetHash().ToString().c_str() << "\n";
                 }
             }
             std::cout << "Mainnet ---\n";
             std::cout << "  nonce: " << genesis.nNonce <<  "\n";
             std::cout << "   time: " << genesis.nTime << "\n";
             std::cout << "   hash: " << genesis.GetHash().ToString().c_str() << "\n";
             std::cout << "   merklehash: "  << genesis.hashMerkleRoot.ToString().c_str() << "\n";
             // Mainnet --- nonce: 296277 time: 1390095618 hash: 000000bdd771b14e5a031806292305e563956ce2584278de414d9965f6ab54b0
         }
         std::cout << std::string("Finished calculating Mainnet Genesis Block:\n");



		
		

        printf("%s\n",consensus.hashGenesisBlock.ToString().c_str());
	    printf("%s\n",genesis.hashMerkleRoot.ToString().c_str());
*/			  
        assert(consensus.hashGenesisBlock == uint256S("0x7530b025d0736556adba955003f326a85c93c9b2d396cfcf8aec7f319d1529b5"));
        assert(genesis.hashMerkleRoot == uint256S("0x5fa45803b90dc3fcb167c7580eb59718d98b08ca2fb26501ea45be0d8fdbfeb7"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        //vSeeds.push_back(CDNSSeedData("127.0.0.1", "127.0.0.1"));
        //vSeeds.push_back(CDNSSeedData("tdkcoin.thecryptochat.net", "tdkcoin.thecryptochat.net"));
		
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,66);   // T
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,8);   // 4
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,13);  // 6
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,134); // L
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (  3, uint256S("0x17a2cc16946ae28ad59f73fa247a3e1f028fef76ad477a363f9349f9b901aa54")),
        };

        chainTxData = ChainTxData{
            // Data as of block 17a2cc16946ae28ad59f73fa247a3e1f028fef76ad477a363f9349f9b901aa54 (height 3).
            1541050486, // * UNIX timestamp of last known number of transactions
            1,  // * total number of transactions between genesis and that timestamp
                    //   (the tx=... number in the SetBestChain debug.log lines)
            0.6     // * estimated number of transactions per second after that timestamp
        };
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 2000000;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 23; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.BIP66Height = 12; // 8075c771ed8b495ffd943980a95f702ab34fce3c8c54e379548bda33cc8c0573
        consensus.powLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 10 * 60; // 10 min
        consensus.nPowTargetSpacing = 5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 6; // 75% for testchains
        consensus.nMinerConfirmationWindow = 8; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1483228800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1517356801; // January 31st, 2018

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1483228800; // January 1, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1517356801; // January 31st, 2018

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0"); //8711

        pchMessageStart[0] = 0x1f;
        pchMessageStart[1] = 0x5f;
        pchMessageStart[2] = 0x5f;
        pchMessageStart[3] = 0xdf;
        nDefaultPort = 33234;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1541005692, 2039180, 0x1e0ffff0, 1, 5 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        printf("%s\n",consensus.hashGenesisBlock.ToString().c_str());
	    printf("%s\n",genesis.hashMerkleRoot.ToString().c_str());
			  
        assert(consensus.hashGenesisBlock == uint256S("0xd6fa5d83cd675ade47ad52fdf91140acec20f81a92c7d5545f3dbbd1c94f20d6"));
        assert(genesis.hashMerkleRoot == uint256S("0x5fa45803b90dc3fcb167c7580eb59718d98b08ca2fb26501ea45be0d8fdbfeb7"));

        vFixedSeeds.clear();
        vSeeds.clear();
		
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

		checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0"))
        };
		
        chainTxData = ChainTxData{
            // Data as of block f2dc531da6be01f53774f970aaaca200c7a8317ee9fd398ee733b40f14e265d1 (height 8702)
            0,
            0,
            0
        };

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.BIP34Height = 100000000; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 25; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 14; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 10 * 60; // 10 min
        consensus.nPowTargetSpacing = 5 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 6; // 75% for testchains
        consensus.nMinerConfirmationWindow = 8; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0x21;
        pchMessageStart[1] = 0x52;
        pchMessageStart[2] = 0x23;
        pchMessageStart[3] = 0x54;
        nDefaultPort = 26211;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1541005692, 2039180, 0x1e0ffff0, 1, 5 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        printf("%s\n",consensus.hashGenesisBlock.ToString().c_str());
	    printf("%s\n",genesis.hashMerkleRoot.ToString().c_str());
			  
        assert(consensus.hashGenesisBlock == uint256S("0xd6fa5d83cd675ade47ad52fdf91140acec20f81a92c7d5545f3dbbd1c94f20d6"));
        assert(genesis.hashMerkleRoot == uint256S("0x5fa45803b90dc3fcb167c7580eb59718d98b08ca2fb26501ea45be0d8fdbfeb7"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true; 

		checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0"))
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SCRIPT_ADDRESS2] = std::vector<unsigned char>(1,58);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
    }

    void UpdateBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
    {
        consensus.vDeployments[d].nStartTime = nStartTime;
        consensus.vDeployments[d].nTimeout = nTimeout;
    }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

void UpdateRegtestBIP9Parameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    regTestParams.UpdateBIP9Parameters(d, nStartTime, nTimeout);
}
