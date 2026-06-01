#pragma once
#include "KamataEngine.h"

namespace MathUtils {

    KamataEngine::Matrix4x4 MakeIdentity4x4() {
        KamataEngine::Matrix4x4 result{};
        for (int i = 0; i < 4; ++i) {
            result.m[i][i] = 1.0f;
        }
        return result;
    }

    KamataEngine::Matrix4x4 Multiply(const KamataEngine::Matrix4x4& a, const KamataEngine::Matrix4x4& b) {
        KamataEngine::Matrix4x4 result = {};
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.m[i][j] = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    result.m[i][j] += a.m[i][k] * b.m[k][j];
                }
            }
        }
        return result;
    }

    KamataEngine::Matrix4x4 MakeScaleMatrix(const KamataEngine::Vector3& scale) {
        KamataEngine::Matrix4x4 scaleMatrix = MakeIdentity4x4();
        scaleMatrix.m[0][0] = scale.x;
        scaleMatrix.m[1][1] = scale.y;
        scaleMatrix.m[2][2] = scale.z;
        return scaleMatrix;
    }

    KamataEngine::Matrix4x4 MakeRotateXMatrix(float angle) {
        KamataEngine::Matrix4x4 rotateXMatrix = MakeIdentity4x4();
        float cosAngle = cosf(angle);
        float sinAngle = sinf(angle);
        rotateXMatrix.m[1][1] = cosAngle;
        rotateXMatrix.m[1][2] = sinAngle;
        rotateXMatrix.m[2][1] = -sinAngle;
        rotateXMatrix.m[2][2] = cosAngle;
        return rotateXMatrix;
    }

    KamataEngine::Matrix4x4 MakeRotateYMatrix(float angle) {
        KamataEngine::Matrix4x4 rotateYMatrix = MakeIdentity4x4();
        float cosAngle = cosf(angle);
        float sinAngle = sinf(angle);
        rotateYMatrix.m[0][0] = cosAngle;
        rotateYMatrix.m[0][2] = -sinAngle;
        rotateYMatrix.m[2][0] = sinAngle;
        rotateYMatrix.m[2][2] = cosAngle;
        return rotateYMatrix;
    }

    KamataEngine::Matrix4x4 MakeRotateZMatrix(float angle) {
        KamataEngine::Matrix4x4 rotateZMatrix = MakeIdentity4x4();
        float cosAngle = cosf(angle);
        float sinAngle = sinf(angle);
        rotateZMatrix.m[0][0] = cosAngle;
        rotateZMatrix.m[0][1] = sinAngle;
        rotateZMatrix.m[1][0] = -sinAngle;
        rotateZMatrix.m[1][1] = cosAngle;
        return rotateZMatrix;
    }


    KamataEngine::Matrix4x4 MakeTranslateMatrix(const KamataEngine::Vector3& translation) {
        KamataEngine::Matrix4x4 translateMatrix = MakeIdentity4x4();
        translateMatrix.m[3][0] = translation.x;
        translateMatrix.m[3][1] = translation.y;
        translateMatrix.m[3][2] = translation.z;
        return translateMatrix;
    }


    KamataEngine::Matrix4x4 MakeAffineMatrix(
        const KamataEngine::Vector3& scale,
        const KamataEngine::Vector3& rotation,
        const KamataEngine::Vector3& translation
    ) {
        KamataEngine::Matrix4x4 result = MakeIdentity4x4();
        KamataEngine::Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

        KamataEngine::Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotation.x);
        KamataEngine::Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotation.y);
        KamataEngine::Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotation.z);

        KamataEngine::Matrix4x4 rotateMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

        KamataEngine::Matrix4x4 translateMatrix = MakeTranslateMatrix(translation);

        // CG2 / DirectX系の行ベクトル前提なら、おそらくこの順番
        result = Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);

        return result;
    }
}