/*
 * arg_parser.h
 *
 *  Created on: 2013/09/22
 *      Author: Turenar (snswinhaiku dot lo at gmail dot com)
 */

#ifndef ARG_PARSER_H_
#define ARG_PARSER_H_

#include <tchar.h>
#include <windows.h>
#include <shellapi.h>

#ifndef UNICODE
# include <stdlib.h>
# include <winnls.h>
#endif

/** コマンドライン引数を解析する
 *
 * @param[in]  args_t  解析する文字列
 * @param[out] *argc   分解された引数文字列の数を返す
 *
 * @retval !=NULL  解析結果（分解された引数を指すポインタ配列）
 * @retval NULL    何らかの原因で解析に失敗
 */
LPTSTR *parse_args(LPCTSTR args_t, int *argc);

/** parse_args()で取得したコマンドライン引数を削除する
 *
 * @param[in] argc   コマンドライン引数の数
 * @param[in] *argv  コマンドライン引数
 */
void free_args(int argc, LPTSTR *argv);

#endif /* ARG_PARSER_H_ */
