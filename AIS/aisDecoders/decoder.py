from pyais import decode

arquivo_txt = "AIS/aisExamples.txt"

with open(arquivo_txt, "r") as f:
    for linha in f:
        linha = linha.strip()
        if not linha:
            continue  # pula linhas vazias
        try:
            mensagemDecodificada = decode(linha)
            parametros = mensagemDecodificada.asdict()
            print("\n--- Nova mensagem ---")
            for chave, valor in parametros.items():
                print(f"{chave}: {valor}")
        except Exception as e:
            print(f"Erro ao decodificar linha: {linha}")
            print("Detalhe do erro:", e)
