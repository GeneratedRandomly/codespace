/**
 * @note 本文件是一个网络请求 wrapper 示例，其作用是将所有网络请求汇总到一个函数内处理
 *       我们推荐你在大作业中也尝试写一个网络请求 wrapper，本文件可以用作参考
 */

import { detectContentType } from "next/dist/server/image-optimizer";
import store from "../redux/store";

export enum NetworkErrorType {
    UNAUTHORIZED,
    REJECTED,
    CORRUPTED_RESPONSE,
    UNKNOWN_ERROR,
}

export class NetworkError extends Error {
    type: NetworkErrorType;
    message: string;

    constructor(
        _type: NetworkErrorType,
        _message: string,
    ) {
        super(_message);

        this.type = _type;
        this.message = _message;
    }

    toString(): string { return this.message; }
    valueOf(): string { return this.message; }
}

export const request = async (
    url: string,
    method: "GET" | "POST" | "PUT" | "DELETE",
    needAuth: boolean,
    body?: object,
) => {
    /**
     * @todo [Step 4] 请在下述两处代码缺失部分以正确根据 `needAuth` 参数读取 JWT 信息并构建请求头
     */
    // Step 4 BEGIN
    const header = new Headers();
    if (needAuth) {
        const state = store.getState();
        const token = state.auth.token;
        header.append("Authorization", `${token}`);
    }
    // Step 4 END

    const response = await fetch(url, {
        method,
        body: body && JSON.stringify(body),

        // Step 4 BEGIN
        headers: header,
        // Step 4 END
    });

    const data = await response.json();
    const code = Number(data.code);

    // HTTP status 401
    if (response.status === 401 && code === 2) {
        throw new NetworkError(
            NetworkErrorType.UNAUTHORIZED,
            "[401] " + data.info,
        );
    }
    else if (response.status === 401) {
        throw new NetworkError(
            NetworkErrorType.CORRUPTED_RESPONSE,
            "[401] " + data.info,
        );
    }

    // HTTP status 403
    if (response.status === 403 && code === 3) {
        throw new NetworkError(
            NetworkErrorType.REJECTED,
            "[403] " + data.info,
        );
    }
    else if (response.status === 403) {
        throw new NetworkError(
            NetworkErrorType.CORRUPTED_RESPONSE,
            "[403] " + data.info,
        );
    }

    // HTTP status 200
    if (response.status === 200 && code === 0) {
        return { ...data, code: undefined };
    }
    else if (response.status === 200) {
        throw new NetworkError(
            NetworkErrorType.CORRUPTED_RESPONSE,
            "[200] " + data.info,
        );
    }

    /**
     * @note 这里的错误处理显然是粗糙的，根据 HTTP status 和 code 的不同应该有更精细的处理
     *       在大作业中，可以尝试编写更为精细的错误处理代码以理清网络请求逻辑
     */
    throw new NetworkError(
        NetworkErrorType.UNKNOWN_ERROR,
        `[${response.status}] ` + data.info,
    );
};